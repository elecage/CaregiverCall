#include "app_priv.h"

#include <driver/gpio.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <esp_matter_attribute.h>
#include <esp_matter_attribute_utils.h>
#include <esp_matter_endpoint.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
static constexpr TickType_t k_button_poll_interval = pdMS_TO_TICKS(20);
static constexpr TickType_t k_button_debounce_time = pdMS_TO_TICKS(80);

extern uint16_t caregiver_endpoint_id;

struct caregiver_driver_t {
    gpio_num_t call_button_gpio;
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

static esp_err_t configure_input(gpio_num_t gpio)
{
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = 1ULL << gpio;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    return gpio_config(&io_conf);
}

static esp_err_t set_feedback_outputs(caregiver_driver_t *driver, bool active)
{
    const uint32_t level = active ? 1 : 0;

    esp_err_t err = gpio_set_level(driver->status_led_gpio, level);
    if (err != ESP_OK) {
        return err;
    }

    err = gpio_set_level(driver->vibration_in1_gpio, level);
    if (err != ESP_OK) {
        return err;
    }

    err = gpio_set_level(driver->vibration_in2_gpio, 0);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "Feedback outputs set to %s", active ? "active" : "idle");
    return err;
}

static bool read_on_off_state(uint16_t endpoint_id)
{
    attribute_t *attribute = attribute::get(endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
    if (!attribute) {
        ESP_LOGW(TAG, "OnOff attribute not found for endpoint %u", endpoint_id);
        return DEFAULT_SWITCH_STATE;
    }

    esp_matter_attr_val_t val = esp_matter_invalid(nullptr);
    esp_err_t err = attribute::get_val(attribute, &val);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Failed to read OnOff attribute: %s", esp_err_to_name(err));
        return DEFAULT_SWITCH_STATE;
    }

    return val.val.b;
}

static void button_poll_task(void *arg)
{
    caregiver_driver_t *driver = static_cast<caregiver_driver_t *>(arg);
    bool was_pressed = gpio_get_level(driver->call_button_gpio) == 0;

    while (true) {
        const bool is_pressed = gpio_get_level(driver->call_button_gpio) == 0;
        if (is_pressed && !was_pressed) {
            vTaskDelay(k_button_debounce_time);
            if (gpio_get_level(driver->call_button_gpio) == 0 && caregiver_endpoint_id != 0) {
                const bool next_state = !read_on_off_state(caregiver_endpoint_id);
                esp_matter_attr_val_t val = esp_matter_bool(next_state);
                esp_err_t err = attribute::update(caregiver_endpoint_id, OnOff::Id,
                                                  OnOff::Attributes::OnOff::Id, &val);
                if (err == ESP_OK) {
                    ESP_LOGI(TAG, "Call button pressed: OnOff updated to %s",
                             next_state ? "true" : "false");
                } else {
                    ESP_LOGE(TAG, "Failed to update OnOff from button press: %s",
                             esp_err_to_name(err));
                }
            }
        }

        was_pressed = is_pressed;
        vTaskDelay(k_button_poll_interval);
    }
}

app_driver_handle_t app_driver_init()
{
    s_driver.call_button_gpio = static_cast<gpio_num_t>(CALL_BUTTON_GPIO);
    s_driver.status_led_gpio = static_cast<gpio_num_t>(STATUS_LED_GPIO);
    s_driver.vibration_in1_gpio = static_cast<gpio_num_t>(VIBRATION_IN1_GPIO);
    s_driver.vibration_in2_gpio = static_cast<gpio_num_t>(VIBRATION_IN2_GPIO);

    esp_err_t err = configure_input(s_driver.call_button_gpio);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure GPIO%d: %s", CALL_BUTTON_GPIO, esp_err_to_name(err));
        return nullptr;
    }

    const gpio_num_t outputs[] = {
        s_driver.status_led_gpio,
        s_driver.vibration_in1_gpio,
        s_driver.vibration_in2_gpio,
    };

    for (gpio_num_t output : outputs) {
        err = configure_output(output);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to configure GPIO%d: %s", output, esp_err_to_name(err));
            return nullptr;
        }
    }

    err = set_feedback_outputs(&s_driver, DEFAULT_SWITCH_STATE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set initial feedback outputs: %s", esp_err_to_name(err));
        return nullptr;
    }

    BaseType_t task_created = xTaskCreate(button_poll_task, "button_poll", 4096, &s_driver, 5, nullptr);
    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create button polling task");
        return nullptr;
    }

    ESP_LOGI(TAG, "Driver initialized: button GPIO%d, LED GPIO%d, MX1508 IN1 GPIO%d, IN2 GPIO%d",
             CALL_BUTTON_GPIO, STATUS_LED_GPIO, VIBRATION_IN1_GPIO, VIBRATION_IN2_GPIO);
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
        if (!val) {
            return ESP_ERR_INVALID_ARG;
        }

        return set_feedback_outputs(driver, val->val.b);
    }

    return ESP_OK;
}
