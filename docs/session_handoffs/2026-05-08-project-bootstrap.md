# 2026-05-08 Project Bootstrap and Product Direction

## Context

CaregiverCall은 ESP32-C3 Super Mini 보드와 Matter를 이용해, 집 안의 지체장애인이 버튼으로 외부 보호자를 호출하는 보조기기 프로젝트입니다.

기본 시험 환경은 Google Nest Mini를 Matter Hub로 사용합니다. 다만 구현은 특정 Google 전용 동작에 묶이지 않고, 다른 Matter Hub와도 연동 가능하도록 Matter 표준 동작을 우선해야 합니다.

## Product Flow

1. 사용자가 물리 버튼을 누릅니다.
2. ESP32-C3 Super Mini 기반 기기가 Matter를 통해 호출 상태를 Matter Hub로 전달합니다.
3. Google Home / Nest Mini 시험 환경에서 보호자의 스마트폰으로 호출 알림을 보냅니다.
4. 보호자가 스마트폰에서 확인 버튼을 누릅니다.
5. 확인 상태가 집 안 시스템으로 전달됩니다.
6. Google Nest Mini가 사용자에게 보호자가 확인했음을 음성으로 알려줍니다.
7. 가능하면 LED, 부저, 진동 등 로컬 피드백도 함께 제공합니다.

## Decisions

- Google Nest Mini는 기본 시험용 Matter Hub이자 사용자 음성 피드백 장치로 사용합니다.
- Google Home 자동화나 Broadcast 기능은 후보 경로로 검토하되, 안전 필수 동작의 유일한 보장 수단으로 의존하지 않습니다.
- 호출 신뢰성, 입력 안정성, 실패 피드백, 접근성을 제품 원칙으로 둡니다.
- Python을 사용하게 되면 저장소 로컬 `.venv` 안에서 실행합니다.
- 클론 후 환경 설정을 위해 `scripts/setup_python_env.ps1`와 `scripts/setup_python_env.sh`를 제공합니다.
- 중요한 작업 후에는 실제 인수인계 문서를 새로 만들고, `SESSION_HANDOFF.md`에는 색인 링크와 짧은 요약만 추가합니다.

## Repository State

- 원격 저장소: `https://github.com/elecage/CaregiverCall.git`
- 작업 브랜치: `codex/project-bootstrap`
- 초기 커밋: `2c05b12 Bootstrap project documentation`

## Key Files

- `AGENTS.md`: 에이전트 작업 원칙, 제품 원칙, Matter 연동 원칙, Python 환경 원칙
- `README.md`: 프로젝트 개요와 Python 환경 설정 안내
- `docs/product_requirements.md`: 호출, 보호자 확인, Nest Mini 피드백, 실패 흐름, 검증 시나리오
- `SESSION_HANDOFF.md`: 인수인계 문서 색인
- `docs/session_handoffs/`: 실제 인수인계 문서 위치
- `scripts/setup_python_env.ps1`: Windows Python VENV 설정
- `scripts/setup_python_env.sh`: macOS/Linux Python VENV 설정
- `requirements.txt`: Python 의존성 기록 위치

## Validation Notes

- `scripts/setup_python_env.ps1`는 실행해 보았으나 현재 머신에는 Python 런처만 있고 실제 Python 설치가 연결되어 있지 않아 `.venv` 생성은 실패했습니다.
- 실패 메시지는 명확하게 나오도록 스크립트를 보강했습니다.
- PowerShell 프로필에서 존재하지 않는 Conda 경로 경고가 나오지만 Git 작업은 정상 수행되었습니다.

## Next Suggestions

- Matter 디바이스 타입과 클러스터 모델링을 결정합니다.
- 호출 상태, 확인 상태, 실패 상태의 상태 머신을 설계합니다.
- ESP-IDF 또는 Arduino 기반 개발 방식을 결정합니다.
- Google Home 자동화 설정 예제 문서를 분리해 작성합니다.

