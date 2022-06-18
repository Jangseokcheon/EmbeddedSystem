
# MQTT + Node_Red

## 개요
이 프로젝트는 MQTT와 Node_Red를 기반으로 만들어진 웹페이지에 rp2040 센서의 값을 출력해주는 프로젝트 입니다.
Node_Red는 JavaScript언어로 만들어진 Flow 기반의 사물 인터넷 와이어링을 위한 시각화 도구 이다.

* rp2040에서 받아오는 목소리,온도,가속도값을 Node_Red 화면에 출력합니다.
* Jetson Nano를 통해 받아온 영상을 Node_Red 화면에 출력합니다.
* Node_Red에서 차트나 그래프를 출력합니다.

이 프로젝트에 사용한 Node_Red 관련 코드는 Node_Red branch에 작성하였습니다.

## 요구 사항
* [Node_Red](https://github.com/Jangseokcheon/EmbeddedSystem/tree/Node_Red)
* [MQTT](https://github.com/Jangseokcheon/EmbeddedSystem/tree/Node_Red)
* Jetson Nano 보드
* rp2040 센서
* 카메라
