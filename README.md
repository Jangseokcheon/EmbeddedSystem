
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
4. 다운로드한 nvm 설치 스크립트를 실행하고 OS 재부팅되도 환경변수 유지되게 설정   
`$ bash install_nvm.sh`   
`$ source ~/.profile`   
5. Node_Red 설치   
`$ nvm install node`
`$ sudo snap install node-red `

## Node_Red DashBoard

![Screenshot from 2022-06-20 01-36-01](https://user-images.githubusercontent.com/86651809/174491498-f642cc6f-d31c-4197-912c-02cdcff28f76.png)

## Node_Red 화면

![cxc](https://user-images.githubusercontent.com/86651809/174577329-6b7ba4f2-d54c-4116-b8c4-40e2947f37db.gif)

![sr3](https://user-images.githubusercontent.com/86651809/174577992-e6379c51-f2d4-475b-b3a0-36a9102edd8e.JPG)

