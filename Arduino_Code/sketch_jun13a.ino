#include <Arduino_LSM6DSOX.h>
#include <ArduinoMqttClient.h>   // C++에서 mqtt를 수행하기 위한 libraries   github에서 다운 로드 가능
#include <WiFiNINA.h>            // mqtt를 사용하기 위해서는 rp2040 역시 wifi에 접속되어 하며 이를 위한 libraries github에서 다운 로드 가능


char ssid[] = "ncs" ;        // your network SSID (name)
char pass[] = "12312300" ;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;  // wifinina 라이브러리 폴더의 src 폴더에 있는 wificlient.h 파일내의 WifiClient 클래스의 생성자를 사용하여 하나의 wificlient 객체를 생성함
MqttClient mqttClient(wifiClient);  // 위에서 생성된 wificlient 객체를 mqttclient 클래스에서 생성되는 mqttclient() 생성자의 argument로 사용함 
WiFiClient wifiClient_gyros;  // wifinina 라이브러리 폴더의 src 폴더에 있는 wificlient.h 파일내의 WifiClient 클래스의 생성자를 사용하여 하나의 wificlient 객체를 생성함
MqttClient mqttClient_gyros(wifiClient);

const char broker[] = "192.168.0.54";   // mqtt_broker가 실행되고 있는 jeston 또는 rapa 의 내부 ip address
int        port     = 1883;            // mqtt_broker의 포트
const char topic[]  = "temperature";       // topic 이름
const char topic_gy[]  = "gyros";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;
//int temperature_deg = 0;
//float x, y, z;
void setup()
{
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
}

void loop()
{
  mqttClient.poll();
  mqttClient_gyros.poll();
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  
  if (IMU.temperatureAvailable() || IMU.accelerationAvailable())
  {
    int temperature_deg = 0;
    float x, y, z;
    IMU.readTemperature(temperature_deg);
    IMU.readAcceleration(x,y,z);

    Serial.print("LSM6DSOX Temperature = ");
    Serial.print(temperature_deg);
    Serial.println(" °C");
    mqttClient.beginMessage(topic);
    mqttClient.print(temperature_deg);
    mqttClient.endMessage();
  

    
    
    mqttClient_gyros.beginMessage(topic_gy);
    if((x> -0.06 && x < 0.05) || (y > -0.06 && y < 0.05) || (z>0.99 && z<1.02))
      mqttClient_gyros.print("정지");
     else if((x> -1.0 && x < 1.0) || (y > -1.0 && y < 1.0) || (z>0.52 && z<-1.48))
      mqttClient_gyros.print("걷기");
     else
     {
      mqttClient_gyros.print("뛰기");
     }
    mqttClient_gyros.endMessage();

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
    
  }
    Serial.println();
   }
}
