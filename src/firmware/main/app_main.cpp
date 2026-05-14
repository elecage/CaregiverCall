#include "app_priv.h"

#include <cstring>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>
#include <esp_matter_providers.h>
#include <crypto/CHIPCryptoPAL.h>
#include <platform/CHIPDeviceEvent.h>
#include <platform/CommissionableDataProvider.h>

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

static const char *TAG = "caregiver_call";
static constexpr auto k_commissioning_window_timeout_seconds = 300;
static constexpr int8_t k_wifi_max_tx_power_quarter_dbm = 34; // 8.5 dBm
static constexpr uint32_t k_setup_passcode = 73948261;
static constexpr uint16_t k_setup_discriminator = 3840;
static constexpr uint32_t k_spake2p_iteration_count = 1000;
static constexpr uint8_t k_spake2p_salt[] = {
    0x53, 0x50, 0x41, 0x4b, 0x45, 0x32, 0x50, 0x20,
    0x4b, 0x65, 0x79, 0x20, 0x53, 0x61, 0x6c, 0x74,
};

uint16_t caregiver_endpoint_id = 0;

class CaregiverCommissionableDataProvider : public chip::DeviceLayer::CommissionableDataProvider {
public:
    CHIP_ERROR GetSetupDiscriminator(uint16_t &setup_discriminator) override
    {
        setup_discriminator = k_setup_discriminator;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR SetSetupDiscriminator(uint16_t setup_discriminator) override
    {
        (void) setup_discriminator;
        return CHIP_ERROR_NOT_IMPLEMENTED;
    }

    CHIP_ERROR GetSpake2pIterationCount(uint32_t &iteration_count) override
    {
        iteration_count = k_spake2p_iteration_count;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSpake2pSalt(chip::MutableByteSpan &salt_buf) override
    {
        if (salt_buf.size() < sizeof(k_spake2p_salt)) {
            return CHIP_ERROR_BUFFER_TOO_SMALL;
        }

        std::memcpy(salt_buf.data(), k_spake2p_salt, sizeof(k_spake2p_salt));
        salt_buf.reduce_size(sizeof(k_spake2p_salt));
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSpake2pVerifier(chip::MutableByteSpan &verifier_buf, size_t &out_verifier_len) override
    {
        chip::Crypto::Spake2pVerifier verifier;
        chip::Crypto::Spake2pVerifierSerialized serialized_verifier = {};
        chip::MutableByteSpan serialized_span(serialized_verifier);
        chip::ByteSpan salt_span(k_spake2p_salt);

        out_verifier_len = sizeof(serialized_verifier);
        if (verifier_buf.size() < out_verifier_len) {
            return CHIP_ERROR_BUFFER_TOO_SMALL;
        }

        ReturnErrorOnFailure(verifier.Generate(k_spake2p_iteration_count, salt_span, k_setup_passcode));
        ReturnErrorOnFailure(verifier.Serialize(serialized_span));

        std::memcpy(verifier_buf.data(), serialized_verifier, out_verifier_len);
        verifier_buf.reduce_size(out_verifier_len);
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetSetupPasscode(uint32_t &setup_passcode) override
    {
        setup_passcode = k_setup_passcode;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR SetSetupPasscode(uint32_t setup_passcode) override
    {
        (void) setup_passcode;
        return CHIP_ERROR_NOT_IMPLEMENTED;
    }
};

static CaregiverCommissionableDataProvider s_commissionable_data_provider;

static void apply_wifi_tx_power_limit()
{
    esp_err_t err = esp_wifi_set_max_tx_power(k_wifi_max_tx_power_quarter_dbm);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Wi-Fi max TX power limited to %d.%02d dBm",
                 k_wifi_max_tx_power_quarter_dbm / 4,
                 (k_wifi_max_tx_power_quarter_dbm % 4) * 25);
        return;
    }

    ESP_LOGW(TAG, "Failed to limit Wi-Fi max TX power: %s", esp_err_to_name(err));
}

static void app_event_cb(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kWiFiConnectivityChange:
        if (event->WiFiConnectivityChange.Result == chip::DeviceLayer::kConnectivity_Established) {
            apply_wifi_tx_power_limit();
        }
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Matter commissioning complete");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        ESP_LOGI(TAG, "Matter commissioning session started");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        ESP_LOGI(TAG, "Matter commissioning session stopped");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Matter commissioning window opened");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Matter commissioning window closed");
        break;
    case chip::DeviceLayer::DeviceEventType::kFabricRemoved:
        ESP_LOGI(TAG, "Matter fabric removed");
        if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
            chip::CommissioningWindowManager &commission_mgr =
                chip::Server::GetInstance().GetCommissioningWindowManager();
            constexpr auto timeout =
                chip::System::Clock::Seconds16(k_commissioning_window_timeout_seconds);
            if (!commission_mgr.IsCommissioningWindowOpen()) {
                CHIP_ERROR err = commission_mgr.OpenBasicCommissioningWindow(
                    timeout, chip::CommissioningWindowAdvertisement::kDnssdOnly);
                if (err != CHIP_NO_ERROR) {
                    ESP_LOGE(TAG, "Failed to reopen commissioning window: %" CHIP_ERROR_FORMAT,
                             err.Format());
                }
            }
        }
        break;
    default:
        break;
    }
}

static esp_err_t app_identification_cb(identification::callback_type_t type,
                                       uint16_t endpoint_id,
                                       uint8_t effect_id,
                                       uint8_t effect_variant,
                                       void *priv_data)
{
    ESP_LOGI(TAG, "Identify callback: type=%u endpoint=%u effect=%u variant=%u",
             type, endpoint_id, effect_id, effect_variant);
    return ESP_OK;
}

static esp_err_t app_attribute_update_cb(attribute::callback_type_t type,
                                         uint16_t endpoint_id,
                                         uint32_t cluster_id,
                                         uint32_t attribute_id,
                                         esp_matter_attr_val_t *val,
                                         void *priv_data)
{
    if (type != attribute::PRE_UPDATE) {
        return ESP_OK;
    }

    return app_driver_attribute_update(static_cast<app_driver_handle_t>(priv_data),
                                       endpoint_id, cluster_id, attribute_id, val);
}

extern "C" void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    app_driver_handle_t driver_handle = app_driver_init();
    ESP_ERROR_CHECK(driver_handle ? ESP_OK : ESP_FAIL);

    node::config_t node_config;
    node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
    ESP_ERROR_CHECK(node ? ESP_OK : ESP_FAIL);

    on_off_plugin_unit::config_t switch_config;
    switch_config.on_off.on_off = DEFAULT_SWITCH_STATE;

    endpoint_t *endpoint = on_off_plugin_unit::create(node, &switch_config, ENDPOINT_FLAG_NONE, driver_handle);
    ESP_ERROR_CHECK(endpoint ? ESP_OK : ESP_FAIL);

    caregiver_endpoint_id = endpoint::get_id(endpoint);
    ESP_LOGI(TAG, "CaregiverCall Matter endpoint created: endpoint_id=%u", caregiver_endpoint_id);

    esp_matter::set_custom_commissionable_data_provider(&s_commissionable_data_provider);

    err = esp_matter::start(app_event_cb);
    ESP_ERROR_CHECK(err);
    apply_wifi_tx_power_limit();

    ESP_ERROR_CHECK(app_driver_set_defaults(caregiver_endpoint_id));

#if CONFIG_ENABLE_CHIP_SHELL
    esp_matter::console::diagnostics_register_commands();
    esp_matter::console::wifi_register_commands();
    esp_matter::console::factoryreset_register_commands();
    esp_matter::console::init();
#endif
}
