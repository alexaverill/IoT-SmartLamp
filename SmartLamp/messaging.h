#ifndef MESSAGING
#define MESSAGING
#include <ArduinoJson.h>
#include "ledcontroller.h";
#include "creds.h";
class Messaging{
public:
  Messaging(LEDController *_led){
    led = _led;  
  }

 void callback(char* topic, byte* payload, unsigned int length) {
//      Serial.print("Message arrived [");
//      Serial.print(topic);
//      Serial.print("] ");
//      for (int i = 0; i < length; i++){
//          Serial.print((char)payload[i]);
//        }
//
//  Serial.println();
  
      StaticJsonDocument<256> doc;
      deserializeJson(doc, payload, length);
      if(doc.containsKey("state")){
          String state = doc["state"];
          if(state == "ON"){
            led->on();
          }else{
            Serial.println("Turn LEDs off");
            led->off();
          }
      }
      if(doc.containsKey("color") == 1){
         int r = doc["color"]["r"];
         int g = doc["color"]["g"];
         int b = doc["color"]["b"];
         led->setColor(r,g,b);
      }
      if(doc.containsKey("brightness")){
        int brightness = doc["brightness"];
        led->setBrightness(brightness);
      }
  }
private:
  LEDController *led;
};
#endif
