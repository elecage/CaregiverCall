#include "app_priv.h"

#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>
#include <platform/CHIPDeviceEvent.h>

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

static const char *TAG = "caregiver_call";
static constexpr auto k_commissioning_window_timeout_seconds = 300;

uint16_t caregiver_endpoint_id = 0;

static void app_event_cb(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
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

    err = esp_matter::start(app_event_cb);
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(app_driver_set_defaults(caregiver_endpoint_id));

#if CONFIG_ENABLE_CHIP_SHELL
    esp_matter::console::diagnostics_register_commands();
    esp_matter::console::wifi_register_commands();
    esp_matter::console::factoryreset_register_commands();
    esp_matter::console::init();
#endif
}
