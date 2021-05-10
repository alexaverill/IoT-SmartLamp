#include<WiFi.h>
#include<PubSubClient.h>
#include<WiFiClientSecure.h>
#include "messaging.h"
#include "creds.h"
#include "ledcontroller.h";


WiFiClient homeAssistClient = WiFiClient();
PubSubClient homeAssistMQTT(homeAssistClient);
Adafruit_NeoPixel strip(24,4, NEO_GRBW + NEO_KHZ800);
LEDController leds(&strip);
Messaging messageHandler(&leds);

void connectToWifi(){
  WiFi.begin(SSID,PWD);
  while(WiFi.status()!= WL_CONNECTED){
    Serial.println("-");
    delay(500);
  }
  Serial.println("Wifi Connected.");
}
 void callback(char* topic, byte* payload, unsigned int length) {
      messageHandler.callback(topic,payload,length);
  }
  
void connectToMQTT(){
    homeAssistMQTT.setServer(HA_ENDPOINT,1883);
    homeAssistMQTT.setCallback(callback);
    Serial.println("Connecting to HA MQTT");
    homeAssistMQTT.connect(DeviceName,HA_USERNAME,HA_PASSWORD);
    
    while (!homeAssistMQTT.connected()) {
      Serial.println("Connecting to  HA MQTT....Retry");
      homeAssistMQTT.connect(DeviceName,HA_USERNAME,HA_PASSWORD);
      delay(5000);
    }
    Serial.println("HA MQTT Connected");
    homeAssistMQTT.subscribe(SubTopic);
}

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWifi();
  connectToMQTT();
  leds.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  homeAssistMQTT.loop();
  leds.update();
}
