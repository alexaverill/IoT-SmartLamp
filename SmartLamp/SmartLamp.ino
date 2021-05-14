#include<WiFi.h>
#include<PubSubClient.h>
#include<WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "messaging.h"
#include "creds.h"
#include "ledcontroller.h";
#include <functional>
WiFiClient homeAssistClient = WiFiClient();
PubSubClient homeAssistMQTT(homeAssistClient);
Adafruit_NeoPixel strip(24,4, NEO_GRBW + NEO_KHZ800);

void SwitchCallback(bool state){
  StaticJsonDocument<256> doc;
  doc["state"]=state?"ON":"OFF";
  char newState[256];
  serializeJson(doc,newState);
  homeAssistMQTT.publish(StateTopic,newState);
}
void ColorCallBack(bool isOn,int r,int g, int b){
  StaticJsonDocument<256> doc;
  doc["state"]=isOn?"ON":"OFF";
  doc["color"]["r"] = r;
  doc["color"]["g"] = g;
  doc["color"]["b"] = b;
  char newState[256];
  serializeJson(doc,newState);
  homeAssistMQTT.publish(StateTopic, newState);
}
void BrightnessCallback(bool isOn,int b){
    StaticJsonDocument<256> doc;
  doc["state"]=isOn?"ON":"OFF";
  doc["brightness"] = b;
  char newState[256];
  serializeJson(doc,newState);
  homeAssistMQTT.publish(StateTopic,newState);
}

LEDController leds(&strip,SwitchCallback,ColorCallBack,BrightnessCallback);
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
    homeAssistMQTT.subscribe(CommandTopic);
}

void setup() {
  
  Serial.begin(9600);
  connectToWifi();
  connectToMQTT();
  leds.start();
  leds.off();
}

void loop() {
  homeAssistMQTT.loop();
  leds.update();
}
