# Importing Libraries
import cv2
import paho.mqtt.client as mqtt                                # mqtt를 사용하기 위한 모듈
import base64                                                  # 영상을  base64로 인코딩 및 디코딩 하기위한 모듈
import mediapipe as mp
import numpy as np

MQTT_BROKER = "localhost"                                        # MQTT_BROKER에 보네줄 본인 아이피 
MQTT_SEND = "image"                                              # MQTT에 보네줄 토픽

max_num_hands = 1                                                # 최대 한개의 손만 검출을 위한 변수 선언
gesture = {
    0:'fist', 1:'one', 2:'two', 3:'three', 4:'four', 5:'five',
    6:'six', 7:'rock', 8:'spiderman', 9:'yeah', 10:'ok',
}
rps_gesture = {0:'rock', 5:'paper', 9:'scissors'}

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
hands = mp_hands.Hands(
    max_num_hands=max_num_hands,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5)

file = np.genfromtxt('gesture_train.csv', delimiter=',')
angle = file[:,:-1].astype(np.float32)
label = file[:, -1].astype(np.float32)
knn = cv2.ml.KNearest_create()
knn.train(angle, cv2.ml.ROW_SAMPLE, label)

cap = cv2.VideoCapture(0)
client = mqtt.Client()
client.connect(MQTT_BROKER)                                       # mqtt client 와 연결
try:
    while True:
        _, img = cap.read()
        img = cv2.flip(img, 1)
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

        result = hands.process(img)

        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

        if result.multi_hand_landmarks is not None:
            for res in result.multi_hand_landmarks:
                joint = np.zeros((21, 3))
                for j, lm in enumerate(res.landmark):
                    joint[j] = [lm.x, lm.y, lm.z]

                v1 = joint[[0,1,2,3,0,5,6,7,0,9,10,11,0,13,14,15,0,17,18,19],:] # Parent joint
                v2 = joint[[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20],:] # Child joint
                v = v2 - v1 # [20,3]

                v = v / np.linalg.norm(v, axis=1)[:, np.newaxis]

                angle = np.arccos(np.einsum('nt,nt->n',
                    v[[0,1,2,4,5,6,8,9,10,12,13,14,16,17,18],:], 
                    v[[1,2,3,5,6,7,9,10,11,13,14,15,17,18,19],:])) # [15,]

                angle = np.degrees(angle) 

                data = np.array([angle], dtype=np.float32)
                ret, results, neighbours, dist = knn.findNearest(data, 3)
                idx = int(results[0][0])

                if idx in rps_gesture.keys():
                    cv2.putText(img, text=rps_gesture[idx].upper(), org=(int(res.landmark[0].x * img.shape[1]), int(res.landmark[0].y * img.shape[0] + 20)),
                     fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=2, color=(255, 255, 255), thickness=3)
                    msg = rps_gesture[idx].upper()
                    client.publish('test', msg)                                   # 검출결과  mqtt 이용해 전달

                mp_drawing.draw_landmarks(img, res, mp_hands.HAND_CONNECTIONS)    
        _, buffer = cv2.imencode('.jpg', img)                                      #이미지 저장을 위한 buffer 
        jpg_text = base64.b64encode(buffer)                                        #이미지를 base64로 인코딩해서 jpg_text에 저장
        client.publish(MQTT_SEND, jpg_text)                                        #jpg_text에 저장한 base64 mqtt를 이용해 전달
except:
    cap.release()
    client.disconnect()
    print("\nNow you can restart fresh")
