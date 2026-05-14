# Google Home 설정 가이드

이 문서는 `보호자 호출기 설정.pptx`의 Google Home 설정 캡처 화면을 기준으로 CaregiverCall 기기를 Google Home에 추가하고, 필요 시 자동화를 구성하는 절차를 정리한 것입니다.

## 준비

- CaregiverCall 기기에 최신 펌웨어가 업로드되어 있어야 합니다.
- 기기 설정 메모리(NVS)를 지운 직후이거나, 아직 Google Home에 등록되지 않은 상태여야 합니다.
- 휴대전화의 Bluetooth와 Wi-Fi를 켭니다.
- 휴대전화와 Google Nest Mini 또는 Matter Hub가 같은 Google Home 집에 등록되어 있어야 합니다.
- 현재 개발용 페어링 코드는 다음과 같습니다.

| 항목 | 값 |
| --- | --- |
| Setup PIN / Passcode | `73948261` |
| Manual pairing code | `35642145134` |
| Discriminator | `3840` |

## 기기 추가

1. Google Home 앱을 열고 상단의 `+` 버튼을 누릅니다.

![Google Home main screen](assets/google_home_setup/01-home-main.jpg)

2. 추가 메뉴에서 `기기`를 선택합니다.

![Google Home add menu](assets/google_home_setup/02-add-menu.jpg)

3. `Google Home에 기기 추가` 화면에서 `기기 검색`을 선택합니다. Google Home이 주변의 Matter 기기를 검색한 뒤, 선택한 기기에 대해 페어링 코드를 입력하는 흐름으로 이어집니다.

![Google Home add device](assets/google_home_setup/03-add-device.jpg)

4. 기기를 추가할 집을 선택하고 `다음`을 누릅니다.

![Select Google Home](assets/google_home_setup/04-select-home.jpg)

5. Google Home이 주변 기기를 검색합니다.

![Searching for device](assets/google_home_setup/05-searching.jpg)

6. 기기가 검색되면 목록의 CaregiverCall 기기를 선택하고 `다음`을 누릅니다. 캡처에서는 개발 중 이름인 `AssistiveDoorlock`으로 표시됩니다.

![Device found](assets/google_home_setup/06-device-found.jpg)

7. Google 계정 연결 및 정보 공유 안내가 표시되면 내용을 확인하고 `동의`를 누릅니다.

![Google account consent](assets/google_home_setup/07-google-consent.jpg)

8. 기기를 찾을 수 없다는 화면이 나오면 전원, Bluetooth, 페어링 코드, 기기 초기화 상태를 확인한 뒤 다시 시도합니다.

![Device not found help](assets/google_home_setup/08-device-not-found-help.jpg)

9. Matter 페어링 코드 입력 화면이 나오면 11자리 manual pairing code `35642145134`를 입력합니다. 앱이 8자리 PIN을 요구하는 화면이면 `73948261`을 입력합니다.

![Matter code entry](assets/google_home_setup/09-matter-code.jpg)

10. iPhone에서 iOS Home 접근 권한 또는 액세서리 추가 확인창이 나오면 허용합니다.

![iOS Home prompt](assets/google_home_setup/10-ios-home-prompt.jpg)

11. 액세서리 카드가 표시되면 계속 진행합니다.

![Accessory card](assets/google_home_setup/11-accessory-card.jpg)

12. 액세서리 이름 입력 화면이 나오면 설치 위치를 알 수 있는 이름을 입력합니다. 예: `보호자호출알림`.

![Accessory name](assets/google_home_setup/12-accessory-name.jpg)

13. 액세서리 추가 확인창이 나오면 `완료` 또는 확인 버튼을 누릅니다.

![Accessory confirmed](assets/google_home_setup/13-accessory-confirm.jpg)

14. 방 선택 화면에서 기기를 둘 방을 선택합니다.

![Select room](assets/google_home_setup/14-select-room.jpg)

15. `기기 연결됨` 화면이 나오면 Google Home 등록이 완료된 것입니다.

![Device connected](assets/google_home_setup/15-device-connected.jpg)

16. 기기 상세 화면에서 이름, 방, 집, 기기 정보를 확인할 수 있습니다.

![Device settings](assets/google_home_setup/16-device-settings.jpg)

## 동작 확인

1. Google Home의 홈 화면에서 CaregiverCall 기기 타일이 보이는지 확인합니다.

![Home control tile](assets/google_home_setup/17-home-control.jpg)

2. 물리 버튼을 한 번 누릅니다.
3. Google Home에 표시되는 On/Off 상태가 바뀌는지 확인합니다.
4. 기기 로컬 피드백이 연결되어 있다면 LED와 진동이 함께 동작하는지 확인합니다.

## 자동화 구성

보호자 알림, 음성 안내, 다른 기기 제어가 필요하면 Google Home의 자동화 기능을 사용합니다.

1. Google Home 앱의 추가 메뉴에서 `자동화` 또는 `자동화 추가`로 들어갑니다.

![Automation menu](assets/google_home_setup/18-automation-menu.jpg)

2. 새 자동화를 추가합니다.

![Add automation](assets/google_home_setup/19-add-automation.jpg)

3. 자동화 이름을 입력합니다. 예: `보호자 호출 알림`.

![Automation name](assets/google_home_setup/20-automation-name.jpg)

4. 시작 조건에서 CaregiverCall 기기의 상태 변화를 선택합니다.

![Automation starters](assets/google_home_setup/21-automation-starters.jpg)

5. 작업 추가 화면에서 알림, 음성 안내, 기기 제어 등 필요한 작업을 선택합니다.

![Automation actions](assets/google_home_setup/22-automation-actions.jpg)

6. 기기 상태 조건을 사용할 경우 CaregiverCall이 `켜짐` 상태가 되었을 때 실행되도록 설정합니다.

![Device state action](assets/google_home_setup/23-device-state-action.jpg)

7. 자동화 저장 후 버튼을 눌러 실제 알림 또는 음성 안내가 실행되는지 확인합니다.

![Automation done](assets/google_home_setup/24-automation-done.jpg)

## 문제 해결

| 증상 | 확인할 내용 |
| --- | --- |
| 기기가 검색되지 않음 | CaregiverCall 전원을 껐다 켠 뒤 1분 안에 다시 시도합니다. 휴대전화 Bluetooth가 켜져 있는지 확인합니다. |
| 코드 입력 후 실패 | 11자리 코드는 `35642145134`, 8자리 PIN은 `73948261`인지 확인합니다. |
| 계속 이전 기기로 인식됨 | Google Home에서 기존 기기를 삭제하고, CaregiverCall의 NVS 설정 메모리를 지운 뒤 다시 추가합니다. |
| `기기를 찾을 수 없음` 화면이 반복됨 | 기기가 CHIPoBLE advertising 상태인지 시리얼 로그에서 확인합니다. |
| 추가 후 버튼 상태가 바뀌지 않음 | 버튼 배선이 `GPIO4`와 `GND` 사이에 연결되어 있는지 확인합니다. |

## COM3 개발 보드 설정 초기화 명령

개발 보드가 COM3로 잡힌 경우, 펌웨어는 유지하고 페어링/Wi-Fi 설정만 지우려면 다음 명령을 사용합니다.

```powershell
D:\GitHub\CaregiverCall\.tools\eim.exe run "esptool.py -p COM3 -b 460800 --before default_reset --after hard_reset --chip esp32c3 erase_region 0x9000 0x6000" v5.4.1 --do-not-track true
```
