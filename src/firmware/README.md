# CaregiverCall Firmware

ESP32-C3 Super Mini firmware for the CaregiverCall Matter device.

## Target

- ESP32-C3
- ESP-IDF `v5.4.1`
- esp-matter `release/v1.4.2` or managed component `espressif/esp_matter` version `1.4.2`
- First milestone: show the device in Google Home as a Matter On/Off switch style device.
- Current commissioning path: Matter over Wi-Fi with NimBLE CHIPoBLE for initial Google Home setup.

## Build

From WSL2 Ubuntu after loading ESP-IDF:

```sh
source ~/esp/esp-idf/export.sh
cd src/firmware
idf.py set-target esp32c3
idf.py build
```

## Flash

If flashing from Windows ESP-IDF shell:

```powershell
idf.py -p COM10 flash monitor
```

If `idf.py flash` cannot open the port, flash the built artifacts directly with esptool:

```powershell
cd src\firmware\build
python -m esptool --chip esp32c3 -p COM3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 bootloader\bootloader.bin 0x20000 caregiver_call.bin 0x8000 partition_table\partition-table.bin 0xf000 ota_data_initial.bin
```

If flashing from WSL2, attach the USB device with `usbipd-win` first and use the Linux device path reported by WSL:

```sh
idf.py -p /dev/ttyACM0 flash monitor
```

## GPIO Defaults

- Call button: `GPIO4`
- External status LED: `GPIO5`
- MX1508 `IN1`: `GPIO6`
- MX1508 `IN2`: `GPIO7`

The call button is active-low with the internal pull-up enabled. A confirmed press toggles the Matter On/Off attribute and sends an Interaction Model report to the paired hub.

Expected serial log:

```text
Call button pressed: OnOff updated to true
Msg TX ... Type 0001:05 (IM:ReportData)
Received status response, status is 0x00
```
