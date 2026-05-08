# 2026-05-08 Windows WSL2 Development Setup

## Context

개발 호스트를 Windows로 확정했고, Matter 개발은 WSL2 Ubuntu에서 진행하기로 했습니다. 로컬에는 WSL 명령은 있으나 Ubuntu 배포판은 아직 설치되지 않은 상태로 확인되었습니다.

## Confirmed Decisions

- 개발 호스트: Windows + WSL2 Ubuntu
- ESP-IDF 버전: `v5.4.1`
- esp-matter 버전: `release/v1.4.2` 또는 ESP Component Registry `espressif/esp_matter` version `1.4.2`
- 기본 업로드 포트: Windows 기준 `COM10`
- USB 장치를 WSL2에 직접 연결해야 하면 `usbipd-win`을 사용합니다.

## Updated Files

- `docs/development_manual.md`: WSL2 설치, Ubuntu 패키지, ESP-IDF, esp-matter, COM10/usbipd 절차 추가
- `docs/development_decisions.md`: 개발 호스트, 버전 고정, COM10 가정 추가
- `scripts/check_windows_dev_env.ps1`: WSL, usbipd, COM 포트 점검 스크립트 추가
- `README.md`: Windows 개발 환경 점검 명령 추가
- `AGENTS.md`: Known Commands에 점검 스크립트 추가

## Notes

- Microsoft 공식 WSL 설치 절차는 관리자 PowerShell에서 `wsl --install`을 사용합니다.
- Espressif esp-matter 공식 문서는 Windows 개발을 WSL2로 안내합니다.
- WSL2에서는 Windows `COM10`이 그대로 Linux 장치로 보이지 않을 수 있으므로, 필요 시 `usbipd-win`으로 USB 장치를 attach해야 합니다.
- `scripts/check_windows_dev_env.ps1` 실행 결과, 현재 로컬은 WSL 명령은 있으나 Ubuntu 배포판이 없고, `usbipd-win`은 설치되어 있지 않으며, 시리얼 포트는 `COM10`으로 확인되었습니다.

## Next Suggestions

- 사용자가 WSL2 Ubuntu를 설치한 뒤 `scripts/check_windows_dev_env.ps1`를 다시 실행합니다.
- Ubuntu에서 ESP-IDF v5.4.1과 esp-matter release/v1.4.2를 설치합니다.
- ESP32-C3 Super Mini가 Windows에서 `COM10`으로 보이는지 확인합니다.
- WSL2에서 플래시할지, Windows ESP-IDF shell에서 플래시할지 실제 환경에서 결정합니다.
