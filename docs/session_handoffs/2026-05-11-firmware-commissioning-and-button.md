# 2026-05-11 Firmware Commissioning and Button Verification

## Summary

COM3 ESP32-C3 Super Mini 보드에서 Google Home Matter 연결과 호출 버튼 입력을 검증했습니다. 초기 실패 원인은 BLE 커미셔닝 비활성화와 버튼 입력 처리 미구현이었습니다.

## Changes

- NimBLE 기반 CHIPoBLE 커미셔닝을 활성화했습니다.
  - `CONFIG_BT_ENABLED=y`
  - `CONFIG_BT_NIMBLE_ENABLED=y`
  - `CONFIG_BT_CONTROLLER_ENABLED=y`
  - `CONFIG_ENABLE_CHIPOBLE=y`
  - `CONFIG_USE_BLE_ONLY_FOR_COMMISSIONING=y`
- ESP32-C3 Super Mini Wi-Fi 안정화를 위해 전력 제한을 적용했습니다.
  - PHY 초기 Wi-Fi TX power: `10 dBm`
  - 런타임 Wi-Fi max TX power: `8.5 dBm`
- `GPIO4` 호출 버튼 입력 처리를 추가했습니다.
  - 내부 pull-up 사용
  - 버튼 누름은 `LOW`
  - 20 ms 폴링, 80 ms 디바운스
  - 입력 확정 시 Matter On/Off 속성 토글
- `GPIO5` LED와 `GPIO6/GPIO7` MX1508 출력은 On/Off 상태에 맞춰 갱신됩니다.

## Verification

빌드:

```powershell
D:\GitHub\CaregiverCall\.tools\eim.exe run "idf.py build" v5.4.1 --do-not-track true
```

업로드:

```powershell
python -m esptool --chip esp32c3 -p COM3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 bootloader\bootloader.bin 0x20000 caregiver_call.bin 0x8000 partition_table\partition-table.bin 0xf000 ota_data_initial.bin
```

확인된 로그:

```text
Configuring CHIPoBLE advertising
NimBLE: GAP procedure initiated: advertise
Msg TX ... Type 0001:05 (IM:ReportData)
Received status response, status is 0x00
Call button pressed: OnOff updated to true
Call button pressed: OnOff updated to false
```

버튼 입력 후 Matter `ReportData`가 전송되고 Google Home 허브에서 `StatusResponse 0x00` 응답을 받는 것을 확인했습니다.

## Notes

- 개발용 Matter 기본 코드는 `20202021`, manual pairing code는 `34970112332`, discriminator는 `3840`입니다.
- 보드가 `DOWNLOAD(USB/UART0/1)`와 `waiting for download` 상태로 부팅하면 앱이 실행되지 않습니다. BOOT 버튼 또는 GPIO9가 GND로 잡혀 있는지 확인해야 합니다.
- 짧은 시간에 버튼을 반복 입력하면 Matter 세션 재전송 로그가 늘어날 수 있습니다. 실제 호출 UX에서는 긴 누름/재입력 잠금 정책을 추가하는 것이 좋습니다.
