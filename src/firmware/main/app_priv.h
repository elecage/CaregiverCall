#pragma once

#include <esp_err.h>
#include <esp_matter.h>
#include <stdint.h>

constexpr bool DEFAULT_SWITCH_STATE = false;

constexpr int CALL_BUTTON_GPIO = 4;
constexpr int STATUS_LED_GPIO = 5;
constexpr int VIBRATION_IN1_GPIO = 6;
constexpr int VIBRATION_IN2_GPIO = 7;

using app_driver_handle_t = void *;

app_driver_handle_t app_driver_init();

esp_err_t app_driver_set_defaults(uint16_t endpoint_id);

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle,
                                      uint16_t endpoint_id,
                                      uint32_t cluster_id,
                                      uint32_t attribute_id,
                                      esp_matter_attr_val_t *val);

