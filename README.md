# CaregiverCall

ESP32-C3 Super Mini 보드와 Matter를 이용한 보호자 호출 보조기기 프로젝트입니다.

## 제품 흐름

사용자가 물리 버튼을 누르면 Matter Hub를 통해 보호자의 스마트폰으로 호출을 보내고, 보호자가 확인 버튼을 누르면 Google Nest Mini가 사용자에게 확인 사실을 알려주는 흐름을 목표로 합니다.

자세한 요구사항은 [docs/product_requirements.md](docs/product_requirements.md)를 참고하세요.

## Python 환경 설정

Python 도구를 사용할 경우 저장소 로컬 가상환경인 `.venv` 안에서 실행합니다.

Windows PowerShell:

```powershell
.\scripts\setup_python_env.ps1
```

macOS/Linux:

```sh
./scripts/setup_python_env.sh
```

Python 패키지 의존성은 `requirements.txt`에 추가합니다.
