# Session Handoff Index

이 파일은 인수인계 문서의 색인입니다. 자세한 내용은 각 인수인계 문서에 기록하고, 이 파일에는 링크와 짧은 요약만 남깁니다.

## Entries

- 2026-05-08: [Firmware skeleton](docs/session_handoffs/2026-05-08-firmware-skeleton.md)
  - `src/firmware/`에 ESP-IDF + esp-matter 프로젝트 골격, Matter On/Off Plugin Unit endpoint, 외부 LED/MX1508 GPIO 드라이버를 추가했습니다.
- 2026-05-08: [Windows WSL2 development setup](docs/session_handoffs/2026-05-08-windows-wsl2-dev-setup.md)
  - Windows native ESP-IDF 우선 개발, 필요 시 WSL2 Ubuntu, ESP-IDF v5.4.1, esp-matter release/v1.4.2, COM10, usbipd-win 준비 절차를 정리했습니다.
- 2026-05-08: [First firmware target](docs/session_handoffs/2026-05-08-first-firmware-target.md)
  - 1차 목표를 Google Home에 Matter On/Off Switch로 표시하는 것으로 고정하고, 외부 LED, 3.3V 소형 진동 모터, MX1508 드라이버와 `GPIO6/7` 제어 핀을 확정했습니다.
- 2026-05-08: [Development decisions](docs/session_handoffs/2026-05-08-development-decisions.md)
  - ESP-IDF + esp-matter, 버튼/스위치 Matter 표현, Google Home 자동화 우선, LED + 진동 피드백, 큰 버튼 1회 입력, 재부팅 시 대기 초기화를 확정했습니다.
- 2026-05-08: [Repository structure and manuals](docs/session_handoffs/2026-05-08-repository-structure-and-manuals.md)
  - `src/`, `lib/`, `docs/` 구조를 추가하고 개발 매뉴얼, Google Home 셋업 매뉴얼, 사용 매뉴얼을 만들었습니다.
- 2026-05-08: [Project bootstrap and product direction](docs/session_handoffs/2026-05-08-project-bootstrap.md)
  - ESP32-C3 Super Mini, Matter, Google Nest Mini 시험 환경, 보호자 확인 피드백, Python VENV 정책, GitHub 푸시 흐름을 정리했습니다.
