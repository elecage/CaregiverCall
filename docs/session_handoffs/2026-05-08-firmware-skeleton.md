# 2026-05-08 Firmware Skeleton

## Context

1차 목표인 Google Home Matter On/Off Switch 표시를 위해 `src/firmware/` 아래에 ESP-IDF + esp-matter 프로젝트 골격을 추가했습니다.

## Added Files

- `src/firmware/CMakeLists.txt`: ESP-IDF 프로젝트 루트
- `src/firmware/README.md`: 빌드와 업로드 안내
- `src/firmware/sdkconfig.defaults`: ESP32-C3, 4MB flash, custom partition, CHIP shell 기본값
- `src/firmware/partitions.csv`: 4MB flash 기준 factory app 파티션
- `src/firmware/main/CMakeLists.txt`: main 컴포넌트 빌드 설정
- `src/firmware/main/idf_component.yml`: ESP-IDF `>=5.4.1,<5.4.2`, `espressif/esp_matter` `1.4.2` 의존성
- `src/firmware/main/app_main.cpp`: Matter node와 On/Off Plugin Unit endpoint 생성
- `src/firmware/main/app_driver.cpp`: 외부 LED와 MX1508 IN1/IN2 GPIO 제어
- `src/firmware/main/app_priv.h`: GPIO와 드라이버 인터페이스 정의

## Behavior

- Matter endpoint는 `on_off_plugin_unit`으로 생성합니다.
- 기본 상태는 OFF입니다.
- On/Off attribute가 바뀌면 외부 LED와 MX1508 IN1이 같은 상태로 바뀝니다.
- MX1508 IN2는 LOW로 유지합니다.
- GPIO 기본값:
  - Call button: `GPIO4` (아직 펌웨어 동작 미구현)
  - External LED: `GPIO5`
  - MX1508 IN1: `GPIO6`
  - MX1508 IN2: `GPIO7`

## Validation

- 로컬 Windows 환경에는 `idf.py`가 없어 실제 빌드는 실행하지 못했습니다.
- 소스 수준으로 오타와 기본 파일 구성을 확인했습니다.
- 첫 실제 검증은 WSL2 Ubuntu에서 `idf.py set-target esp32c3 && idf.py build`로 진행해야 합니다.

## Next Steps

- WSL2 Ubuntu와 ESP-IDF v5.4.1, esp-matter v1.4.2 설치 후 빌드를 실행합니다.
- `on_off_plugin_unit`이 Google Home에서 원하는 스위치 형태로 표시되는지 확인합니다.
- 빌드 에러가 나면 esp-matter v1.4.2 API에 맞춰 endpoint 타입 또는 include를 조정합니다.
- 1차 목표가 통과된 뒤 GPIO4 버튼 입력, 디바운스, 자동 OFF, 진동 pulse 패턴을 추가합니다.
