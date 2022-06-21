#include <Arduino_LSM6DSOX.h>
#include <ArduinoMqttClient.h>   // C++에서 mqtt를 수행하기 위한 libraries   github에서 다운 로드 가능
#include <WiFiNINA.h>            // mqtt를 사용하기 위해서는 rp2040 역시 wifi에 접속되어 하며 이를 위한 libraries github에서 다운 로드 가능
///////////////////////////////////////////////
#include <PDM.h>
///////////////////////////////////////////////

///////////////////////////////////////////////
bool LED_SWITCH = false;
 
// default number of output channels
static const char channels = 1;
 
// default PCM output frequency 
static const int frequency = 16000;
 
// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[512];
 
// Number of audio samples read
volatile int samplesRead;
///////////////////////////////////////////////

char ssid[] = "ncs" ;        // your network SSID (name)
char pass[] = "12312300" ;    // your network password (use for WPA, or use as key for WEP)
WiFiClient wifiClient;  // wifinina 라이브러리 폴더의 src 폴더에 있는 wificlient.h 파일내의 WifiClient 클래스의 생성자를 사용하여 하나의 wificlient 객체를 생성함
MqttClient mqttClient(wifiClient);  // 위에서 생성된 wificlient 객체를 mqttclient 클래스에서 생성되는 mqttclient() 생성자의 argument로 사용함 
WiFiClient wifiClient_gyros;  // wifinina 라이브러리 폴더의 src 폴더에 있는 wificlient.h 파일내의 WifiClient 클래스의 생성자를 사용하여 하나의 wificlient 객체를 생성함
MqttClient mqttClient_gyros(wifiClient);
WiFiClient wifiClient_voice;  // wifinina 라이브러리 폴더의 src 폴더에 있는 wificlient.h 파일내의 WifiClient 클래스의 생성자를 사용하여 하나의 wificlient 객체를 생성함
MqttClient mqttClient_voice(wifiClient_voice);

const char broker[] = "192.168.0.54";   // mqtt_broker가 실행되고 있는 jeston 또는 rapa 의 내부 ip address
int        port     = 1883;            // mqtt_broker의 포트
const char topic[]  = "temperature";       // topic 이름
const char topic_gy[]  = "gyros";
const char topic_vi[]  = "voice";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;
int temperature_deg = 0;
float x, y, z;
void setup()
{
  Serial.begin(9600);
  ///////////////////////////////////////////////
  pinMode(LEDB, OUTPUT);
  ///////////////////////////////////////////////
  while (!Serial);
///////////////////////////////////////////////
  PDM.onReceive(onPDMdata);
   if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
///////////////////////////////////////////////
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
mqttClient_voice.poll();
 if (samplesRead) {
 
    // Print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
      if (channels == 2) {
        Serial.print("L:");
        Serial.print(sampleBuffer[i]);
        Serial.print(" R:");
        i++;
      }
      if (sampleBuffer[i] > 10000 || sampleBuffer[i] <= -10000) {
        LED_SWITCH = !LED_SWITCH;
        if (LED_SWITCH) {
          mqttClient_voice.beginMessage(topic);
           mqttClient_voice.print("go");
          mqttClient_voice.endMessage();
           Serial.println(sampleBuffer[i]);
        }
        else {
          delay(1000);
        }
      }
    }
 
    // Clear the read count
    samplesRead = 0;
  }
  
  
  mqttClient_gyros.poll();
  mqttClient.poll();
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  
  if (IMU.temperatureAvailable())
  {
    //int temperature_deg = 0;
    IMU.readTemperature(temperature_deg);

    Serial.print("LSM6DSOX Temperature = ");
    Serial.print(temperature_deg);
    Serial.println(" °C");
  }

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

    mqttClient_gyros.beginMessage(topic_gy);
    mqttClient_gyros.print(x);
    mqttClient_gyros.print(" ");
    mqttClient_gyros.print(y);
    mqttClient_gyros.print(" ");
    mqttClient_gyros.print(z);
    mqttClient_gyros.endMessage();
  }

    
  Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.print("hello ");
    Serial.println(count);


    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(temperature_deg);
    mqttClient.endMessage();

    Serial.println();

    count++;

    
  }

}

///////////////////////////////////////////////
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();
 
  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);
 
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}
///////////////////////////////////////////////
