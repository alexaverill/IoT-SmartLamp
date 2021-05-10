#ifndef MESSAGING
#define MESSAGING
#include <ArduinoJson.h>
#include "ledcontroller.h";
class Messaging{
public:
  Messaging(LEDController *_led){
    led = _led;  
  }
 void callback(char* topic, byte* payload, unsigned int length) {
      Serial.print("Message arrived [");
      Serial.print(topic);
      Serial.print("] ");
      StaticJsonDocument<256> doc;
      deserializeJson(doc, payload, length);
      String type = doc["type"];
      Serial.println(type);
      if(type=="toggle"){
        Serial.println("Toggling LEDs");
        led->toggle();
      }else if(type =="color"){
        int r = doc["r"];
        int g = doc["g"];
        int b = doc["b"];
        int w = doc["w"];
        int bright = doc["brightness"];
        led->setColor(r,g,b,w,bright);
      }
  }
private:
  LEDController *led;
};
#endif
