#include<WiFi.h>
#include<PubSubClient.h>
#include<WiFiClientSecure.h>
#include "messaging.h"
#include "creds.h"
Messaging messageHandler;
WiFiClient homeAssistClient = WiFiClient();
PubSubClient homeAssistMQTT(homeAssistClient);


void connectToWifi(){
  WiFi.begin(SSID,PWD);
  while(WiFi.status()!= WL_CONNECTED){
    Serial.println("-");
    delay(500);
  }
  Serial.println("Wifi Connected.");
}
 void callback(char* topic, byte* payload, unsigned int length) {
      Serial.print("Message arrived [");
      Serial.print(topic);
      Serial.print("] ");
      for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
      }
      Serial.println();
    
      // Switch on the LED if an 1 was received as first character
      if ((char)payload[0] == '1') {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is active low on the ESP-01)
      } else {
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
      }
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
}

void loop() {
  // put your main code here, to run repeatedly:
  homeAssistMQTT.loop();
}
