# 2026-05-08 Repository Structure and Manuals

## Context

저장소를 코드, 문서, 라이브러리 디렉터리로 구분하고, 문서 디렉터리에 개발 매뉴얼, Google Home 셋업 매뉴얼, 실제 사용 매뉴얼을 추가했습니다.

## Changes

- `src/` 디렉터리를 펌웨어 소스 코드 위치로 추가했습니다.
- `lib/` 디렉터리를 프로젝트 내부 라이브러리 또는 외부 라이브러리 래퍼 위치로 추가했습니다.
- `docs/development_manual.md`를 추가했습니다.
- `docs/setup_manual.md`를 추가했습니다.
- `docs/user_manual.md`를 추가했습니다.
- `README.md`, `AGENTS.md`, `docs/product_requirements.md`에서 새 문서와 디렉터리 구조를 연결했습니다.

## Manual Contents

- 개발 매뉴얼은 환경설정, 컴파일, 업로드 섹션과 ESP32-C3 Super Mini 초기 결선도 및 결선 표를 포함합니다.
- 셋업 매뉴얼은 Google Home 앱에서 Matter 기기를 추가하고 Nest Mini로 확인 안내를 시험하는 흐름을 설명합니다.
- 사용 매뉴얼은 사용자와 보호자가 실제로 호출, 확인, 음성 안내를 사용하는 흐름을 설명합니다.

## Hardware Assumptions

초기 결선 기준은 다음과 같습니다.

- 호출 버튼: `GPIO4`
- 상태 LED: `GPIO5`
- 부저 또는 진동 모터 제어 신호: `GPIO6`

ESP32-C3 Super Mini 보드는 판매처나 제조사에 따라 핀 표기가 다를 수 있으므로 실제 배선 전 보드 실크스크린과 핀맵을 확인해야 합니다.

## Next Suggestions

- 실제 사용할 ESP32-C3 Super Mini 보드 사진 또는 핀맵을 기준으로 결선도를 확정합니다.
- 펌웨어 스택을 ESP-IDF 또는 Arduino 중 하나로 결정합니다.
- 컴파일과 업로드 명령을 확정한 뒤 `docs/development_manual.md`에 실제 명령을 추가합니다.
- Google Home 자동화 예제를 별도 문서로 분리할지 결정합니다.

