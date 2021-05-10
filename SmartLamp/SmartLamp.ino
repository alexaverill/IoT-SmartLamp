#include<WiFi.h>
#include<PubSubClient.h>
#include<WiFiClientSecure.h>

#include "creds.h"

WiFiClient homeAssistantClient = WiFiClient();
PubSubClient homeAssistantMQTT(homeAssistantClient);

void connectToWifi(){
  WiFi.begin(SSID,PWD);
  while(WiFi.status()!= WL_CONNECTED){
    Serial.println("-");
    delay(500);
  }
  Serial.println("Wifi Connected.");
}

void connectToMQTT(){
    homeAssistMQTT.setServer(HA_ENDPOINT,1883);
    Serial.println("Connecting to HA MQTT");
    homeAssistMQTT.connect(DeviceName,HA_USERNAME,HA_PASSWORD);
    
    while (!homeAssistMQTT.connected()) {
      Serial.println("Connecting to  HA MQTT....Retry");
      homeAssistMQTT.connect(DeviceName,HA_USERNAME,HA_PASSWORD);
      delay(5000);
    }
    Serial.println("HA MQTT Connected");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWifi();
  connectToMQTT();
}

void loop() {
  // put your main code here, to run repeatedly:

}
