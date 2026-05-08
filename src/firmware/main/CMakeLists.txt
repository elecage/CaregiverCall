idf_component_register(
    SRCS
        "app_driver.cpp"
        "app_main.cpp"
    INCLUDE_DIRS
        "."
    REQUIRES
        driver
        esp_matter
        nvs_flash
)

