#include "app_priv.h"

#include <driver/gpio.h>
#include <esp_log.h>

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <esp_matter_attribute.h>
#include <esp_matter_endpoint.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";

extern uint16_t caregiver_endpoint_id;

struct caregiver_driver_t {
    gpio_num_t status_led_gpio;
    gpio_num_t vibration_in1_gpio;
    gpio_num_t vibration_in2_gpio;
};

static caregiver_driver_t s_driver = {};

static esp_err_t configure_output(gpio_num_t gpio)
{
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = 1ULL << gpio;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    return gpio_config(&io_conf);
}

static esp_err_t set_feedback_outputs(caregiver_driver_t *driver, bool active)
{
    esp_err_t err = ESP_OK;
    const uint32_t level = active ? 1 : 0;

    err |= gpio_set_level(driver->status_led_gpio, level);
    err |= gpio_set_level(driver->vibration_in1_gpio, level);
    err |= gpio_set_level(driver->vibration_in2_gpio, 0);

    ESP_LOGI(TAG, "Feedback outputs set to %s", active ? "active" : "idle");
    return err;
}

app_driver_handle_t app_driver_init()
{
    esp_err_t err = ESP_OK;

    s_driver.status_led_gpio = static_cast<gpio_num_t>(STATUS_LED_GPIO);
    s_driver.vibration_in1_gpio = static_cast<gpio_num_t>(VIBRATION_IN1_GPIO);
    s_driver.vibration_in2_gpio = static_cast<gpio_num_t>(VIBRATION_IN2_GPIO);

    err |= configure_output(s_driver.status_led_gpio);
    err |= configure_output(s_driver.vibration_in1_gpio);
    err |= configure_output(s_driver.vibration_in2_gpio);
    err |= set_feedback_outputs(&s_driver, DEFAULT_SWITCH_STATE);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize GPIO outputs: %s", esp_err_to_name(err));
        return nullptr;
    }

    ESP_LOGI(TAG, "Driver initialized: LED GPIO%d, MX1508 IN1 GPIO%d, IN2 GPIO%d",
             STATUS_LED_GPIO, VIBRATION_IN1_GPIO, VIBRATION_IN2_GPIO);
    return &s_driver;
}

esp_err_t app_driver_set_defaults(uint16_t endpoint_id)
{
    caregiver_driver_t *driver = static_cast<caregiver_driver_t *>(endpoint::get_priv_data(endpoint_id));
    if (!driver) {
        return ESP_ERR_INVALID_STATE;
    }

    attribute_t *attribute = attribute::get(endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
    if (!attribute) {
        return ESP_ERR_NOT_FOUND;
    }

    esp_matter_attr_val_t val = esp_matter_invalid(nullptr);
    esp_err_t err = attribute::get_val(attribute, &val);
    if (err != ESP_OK) {
        return err;
    }

    return set_feedback_outputs(driver, val.val.b);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle,
                                      uint16_t endpoint_id,
                                      uint32_t cluster_id,
                                      uint32_t attribute_id,
                                      esp_matter_attr_val_t *val)
{
    if (endpoint_id != caregiver_endpoint_id) {
        return ESP_OK;
    }

    caregiver_driver_t *driver = static_cast<caregiver_driver_t *>(driver_handle);
    if (!driver) {
        return ESP_ERR_INVALID_ARG;
    }

    if (cluster_id == OnOff::Id && attribute_id == OnOff::Attributes::OnOff::Id) {
        return set_feedback_outputs(driver, val->val.b);
    }

    return ESP_OK;
}
