
# MQTT + Node_Red

## 개요
이 프로젝트는 MQTT와 Node_Red를 기반으로 만들어진 웹페이지에 rp2040 센서의 값을 출력해주는 프로젝트 입니다.
Node_Red는 JavaScript언어로 만들어진 Flow 기반의 사물 인터넷 와이어링을 위한 시각화 도구 이다.

* rp2040에서 받아오는 목소리,온도,가속도값을 Node_Red 화면에 출력합니다.
* Jetson Nano를 통해 받아온 영상을 Node_Red 화면에 출력합니다.
* Node_Red에서 차트나 그래프를 출력합니다.

이 프로젝트에 사용한 Node_Red 관련 코드는 [Node_Red](https://github.com/Jangseokcheon/EmbeddedSystem/tree/Node_Red) branch에 작성하였습니다.

## 요구 사항
* [Node_Red](https://github.com/Jangseokcheon/EmbeddedSystem/tree/Node_Red)
* [MQTT](https://github.com/Jangseokcheon/EmbeddedSystem/tree/Node_Red)
* Jetson Nano 보드
* rp2040 센서
* 카메라

## 주제 수행 내용
* rp2040에 달려있는 마이크를 통해 사용자의 음성을 인식하여 음성으로 파장 Node_Red에 출력
* rp2040에 달려있는 가속도 센서를 이용하여 사용자가 걷는지 달리는지 멈춰있는지에 대한 상태 Node_Red에 출력
* rp2040에 달려있는 온도 센서를 이용하여 현재 온도를 Node_Red 화면에 출력
* Jetson Nano에 달려 있는 카메라를 이용하여 사용자가 걷고 있는지 멈춰 있는지 파악할 수 있게 영상 스트리밍
* 위에서 받아온 정보들을 가공하여 Node_Red Ui 화면 구성

## 블록도
![easd](https://user-images.githubusercontent.com/86651809/174460050-2d7856cc-2cd5-4d56-9b1d-7b14f558d535.JPG)

## 구현 영상

https://user-images.githubusercontent.com/86651809/174586525-659aa813-b212-4624-a89b-2a19345076d0.mp4

