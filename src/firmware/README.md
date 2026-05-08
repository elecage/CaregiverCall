# CaregiverCall Firmware

ESP32-C3 Super Mini firmware for the CaregiverCall Matter device.

## Target

- ESP32-C3
- ESP-IDF `v5.4.1`
- esp-matter `release/v1.4.2` or managed component `espressif/esp_matter` version `1.4.2`
- First milestone: show the device in Google Home as a Matter On/Off switch style device.

## Build

From WSL2 Ubuntu after loading ESP-IDF:

```sh
source ~/esp/esp-idf/export.sh
cd /mnt/c/Users/hmshim/.codex/worktrees/ef01/CaregiverCall/src/firmware
idf.py set-target esp32c3
idf.py build
```

## Flash

If flashing from Windows ESP-IDF shell:

```powershell
idf.py -p COM10 flash monitor
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
