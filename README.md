
# Node_Red 빌드

## 개요
이 프로젝트는 Jetson Nano 즉 리눅스 환경에서 Node_Red를 사용하게 하기 위한 프로젝트입니다.

* MQTT와 Node_Red 설치
* 설치후 Node_Red Ui 출력

## 요구 사항
* Jetson Nano 보드

## 설치
1. 중간에서 발신자,수신자를 연결해 줄 브로커 설치(Mosquitto)   
`$ sudo apt install mosquitto -y`   
2. 클라이언트 설치   
`$ sudo apt install mosquitto-clients`   
3. Node.js 다운로드   
`$ sudo apt install nodejs -y`   
4. nvm 설치스크립트를 다운로드   
`$ curl -sL https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh –o install_nvm.sh`   
