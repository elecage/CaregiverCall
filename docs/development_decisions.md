# Development Decisions

이 문서는 구현 시작 전에 확정한 개발 결정을 기록합니다.

## 확정된 결정

| 항목 | 결정 |
| --- | --- |
| 펌웨어 스택 | ESP-IDF + esp-matter |
| 개발 호스트 | Windows + WSL2 Ubuntu |
| ESP-IDF 버전 | v5.4.1 |
| esp-matter 버전 | release/v1.4.2 또는 ESP Component Registry `espressif/esp_matter` version `1.4.2` |
| 기본 업로드 포트 | Windows 기준 `COM10` |
| 1차 개발 목표 | Matter 스위치로 Google Home에 표시 |
| 기본 Matter 표현 | On/Off Switch |
| 보호자 확인 흐름 | 가급적 Google Home 자동화만 사용 |
| 기본 시험 Hub | Google Nest Mini |
| 사용자 피드백 | 외부 LED + 3.3V 소형 진동 모터, 가능하면 Google Nest Mini 음성 안내 |
| 진동 모터 드라이버 | MX1508 모터 드라이버 |
| MX1508 제어 핀 | `GPIO6 -> IN1`, `GPIO7 -> IN2` |
| 버튼 입력 | 큰 버튼 1회 입력 |
| 입력 안정화 | 디바운스와 중복 호출 방지 강화 |
| 재부팅 후 상태 | 대기 상태로 초기화 |
| 개발 보드 | ESP32-C3 Super Mini |

## 현재 보드 기준

사용자가 제공한 ESP32-C3 Super Mini 보드 이미지를 기준으로 개발합니다.

- 참조 이미지: https://europe1.discourse-cdn.com/arduino/optimized/4X/b/4/1/b41cb5d47221f72dce90d2227369a7aa359fa2d0_2_690x389.jpeg
- 보드 핀 배치는 제조사와 판매처에 따라 달라질 수 있으므로 실제 배선 전 보드 실크스크린을 확인합니다.
- MX1508 모터 드라이버 모듈 참조 이미지: https://ae01.alicdn.com/kf/Scd3bb02492b64cd89a5f1e06951d5baaK.jpg

## 초기 구현 방향

- 먼저 Matter 기기가 Google Home에서 On/Off Switch 형태로 인식되도록 최소 펌웨어를 구성합니다.
- 1차 목표에서는 Google Home에 Matter 스위치로 표시되는 것까지만 검증합니다.
- Windows에서는 WSL2 Ubuntu 안에서 ESP-IDF와 esp-matter를 사용합니다.
- 보드 업로드는 Windows `COM10`을 기본 가정으로 시작하되, WSL2에서 직접 플래시해야 하면 `usbipd-win`으로 USB 장치를 연결합니다.
- 버튼을 한 번 누르면 호출 상태가 활성화됩니다.
- 호출 상태는 중복 전송을 막기 위해 짧은 시간 동안 재입력을 무시합니다.
- 재부팅하면 이전 호출 상태를 복원하지 않고 대기 상태로 시작합니다.
- 외부 LED는 대기, 호출 전송 중, 호출 실패, 확인 완료 상태를 표시하는 후보 출력입니다.
- 3.3V 소형 진동 모터는 MX1508 모터 드라이버의 한 채널을 통해 구동합니다.
- 진동 모터 제어는 `GPIO6 -> IN1`, `GPIO7 -> IN2`, 모터는 `OUT1/OUT2`에 연결하는 것을 기본으로 합니다.
- 보호자 확인과 Nest Mini 음성 안내는 Google Home 자동화로 먼저 검증합니다.
- 나머지 세부 동작은 진행하면서 결정합니다.

## 아직 확인이 필요한 항목

1. Google Home 자동화만으로 보호자 스마트폰의 "확인 버튼"을 실제로 구현할 수 있는지 검증해야 합니다.
2. On/Off Switch 표현이 Google Home 자동화 트리거와 사용자 경험에 충분한지 실험해야 합니다.
3. 온보드 LED를 디버그용으로 함께 사용할지 결정해야 합니다.
4. MX1508 모터 드라이버 모듈의 실제 핀 라벨이 문서의 `IN1/IN2/OUT1/OUT2/+/-` 가정과 같은지 실물로 확인해야 합니다.
5. 보호자 확인이 일정 시간 안에 오지 않을 때 재알림을 할지, 단순 미확인 상태로 둘지 결정해야 합니다.
6. Google Home 자동화가 부족할 경우 별도 웹앱/서버 사용을 허용할지 결정해야 합니다.
