# 2026-05-14 Pairing Code Update

## Summary

The Matter commissioning PIN was changed because the previous development PIN overlapped with an existing door-lock PIN. The firmware now uses a CaregiverCall-specific custom commissionable data provider instead of the default Matter test passcode.

## Current Pairing Codes

| Item | Value |
| --- | --- |
| Setup PIN / Passcode | `73948261` |
| Manual pairing code | `35642145134` |
| Discriminator | `3840` |

The 11-digit manual pairing code is derived from the setup PIN and discriminator. With setup PIN `73948261` and discriminator `3840`, the manual pairing code is `35642145134`.

## Firmware Changes

- `src/firmware/main/app_main.cpp` adds `CaregiverCommissionableDataProvider`.
- The provider returns setup PIN `73948261`, discriminator `3840`, SPAKE2+ iteration count `1000`, SPAKE2+ salt `SPAKE2P Key Salt`, and a verifier generated at runtime for the configured PIN.
- `src/firmware/sdkconfig.defaults` selects `CONFIG_CUSTOM_COMMISSIONABLE_DATA_PROVIDER=y`.

## Verification

- Firmware build completed successfully.
- New app binary was generated:
  - `src/firmware/build/caregiver_call.bin`
  - size: `1835792` bytes
  - build time: `2026-05-14 17:06:21`
- COM3 upload completed successfully.
  - Port: `COM3`
  - MAC: `e0:72:a1:68:a0:dc`
  - bootloader, partition table, OTA data, and app all passed hash verification.
- COM3 NVS settings memory was erased after flashing.
- Serial boot log confirmed CHIPoBLE advertising:

```text
Configuring CHIPoBLE advertising
NimBLE: GAP procedure initiated: advertise
```

## Follow-Up Notes

- Existing Google Home pairing state must be removed before adding the device again.
- If only settings need to be reset, erase NVS only:

```powershell
D:\GitHub\CaregiverCall\.tools\eim.exe run "esptool.py -p COM3 -b 460800 --before default_reset --after hard_reset --chip esp32c3 erase_region 0x9000 0x6000" v5.4.1 --do-not-track true
```
