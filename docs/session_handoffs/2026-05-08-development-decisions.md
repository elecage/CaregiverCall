# 2026-05-08 Development Decisions

## Context

구현 시작 전 사용자 확인을 받아 핵심 개발 방향을 확정했습니다.

## Confirmed Decisions

- 펌웨어 스택은 ESP-IDF + esp-matter를 사용합니다.
- Matter 기기는 Google Home에서 버튼 또는 스위치처럼 보이게 하는 방향으로 검증합니다.
- 보호자 확인 흐름은 가급적 Google Home 자동화만 사용합니다.
- 사용자 피드백은 LED + 진동을 기본 후보로 두고, 가능하면 Google Nest Mini 음성 안내를 함께 사용합니다.
- 호출 입력은 큰 버튼 1회 입력으로 처리합니다.
- 디바운스와 중복 호출 방지를 강화합니다.
- 재부팅하면 이전 호출 상태를 복원하지 않고 대기 상태로 초기화합니다.
- ESP32-C3 Super Mini 보드 이미지는 사용자가 제공한 URL을 참조합니다.

## Updated Files

- `docs/development_decisions.md`: 확정 결정과 남은 확인 항목
- `docs/product_requirements.md`: Matter 표현과 재부팅 상태 정책 반영
- `docs/development_manual.md`: ESP-IDF + esp-matter, 버튼 입력, 진동 피드백, 보드 이미지 URL 반영
- `docs/setup_manual.md`: Google Home 자동화 우선과 검증 필요성 반영
- `docs/user_manual.md`: 큰 버튼 1회 입력과 진동 피드백 반영
- `AGENTS.md`: 구현 원칙과 관련 문서 갱신 규칙 반영

## Still Needs User Confirmation

- Google Home 자동화만으로 보호자 스마트폰의 확인 버튼 흐름이 실제로 가능한지 실기 검증이 필요합니다.
- Google Home에서 버튼 표현과 스위치 표현 중 어느 쪽이 자동화와 UX에 더 나은지 실험해야 합니다.
- 외부 LED와 진동 모터 부품, 드라이버 회로를 확정해야 합니다.
- 확인 응답이 없을 때 재알림을 할지, 미확인 상태만 유지할지 결정해야 합니다.
- Google Home 자동화만으로 부족할 경우 별도 웹앱/서버를 허용할지 결정해야 합니다.

