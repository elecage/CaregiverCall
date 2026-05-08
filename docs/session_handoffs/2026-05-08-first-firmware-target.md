# 2026-05-08 First Firmware Target

## Context

1차 개발 목표와 하드웨어 피드백 부품을 추가로 확정했습니다.

## Confirmed Decisions

- 1차 목표는 Matter 스위치로 Google Home에 표시되는 것까지만 진행합니다.
- 기본 Matter 표현은 On/Off Switch입니다.
- 사용자 피드백은 외부 LED와 3.3V 소형 진동 모터를 사용합니다.
- 진동 모터 드라이버는 MX1508 모터 드라이버를 사용합니다.
- MX1508은 한 채널만 사용하며 기본 배선은 `GPIO6 -> IN1`, `GPIO7 -> IN2`, 모터는 `OUT1/OUT2`입니다.
- 나머지 세부 동작은 개발을 진행하면서 결정합니다.

## Updated Files

- `docs/development_decisions.md`: 1차 목표, On/Off Switch, 외부 LED, 3.3V 진동 모터, MX1508 반영
- `docs/development_manual.md`: 1차 펌웨어 목표와 MX1508 기준 결선 설명 추가
- `docs/product_requirements.md`: 1차 Matter 목표를 On/Off Switch로 명확화
- `docs/user_manual.md`: 외부 LED 표현으로 상태 안내 조정
- `AGENTS.md`: 1차 Matter 목표 반영

## Remaining Decisions

- Google Home 자동화만으로 보호자 확인 버튼 흐름을 구현할 수 있는지 실기 검증이 필요합니다.
- MX1508 모듈의 실제 핀 라벨이 문서의 `IN1/IN2/OUT1/OUT2/+/-` 가정과 일치하는지 실물로 확인해야 합니다.
- 온보드 LED를 디버그용으로 사용할지 결정해야 합니다.
- 재알림, 호출 자동 해제 시간, 중복 방지 시간은 진행하면서 결정합니다.
