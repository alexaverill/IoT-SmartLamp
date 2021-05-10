#ifndef LEDCONTROLLER
#define LEDCONTROLLER

#include <Adafruit_NeoPixel.h>

//int (*fcnPtr)(int){ &foo }; // Initialize fcnPtr with function foo
 //   (*fcnPtr)(5); // call function foo(5) through fcnPtr.
 //int (Data::*fp1) (float) = &Data::f; 
class LEDController{
  public:
    LEDController(Adafruit_NeoPixel *_strip){
      strip = _strip;     
    }
    
    void start(){
      strip->begin();
      strip->show();
      strip->setBrightness(brightness); 
      //off();
    }
    
    void update(){
        if(!needsRefresh){ return;}  
        (*this.*internalUpdate)();
        
    }
    void setColor(int _r, int _g, int _b, int _w, int _brightness){
      r = _r;
      g = _g;
      b = _b;
      w = _w;
      brightness= _brightness;
      setBrightness();
      on();
      internalUpdate =&LEDController::colorUpdate;
      needsRefresh = true;
    }
    
    void colorUpdate(){
        Serial.println("Updating LEDs");
        strip->fill(strip->Color(r,g,b,w));
        strip->show();
        needsRefresh= false;
    }

    void off(){
      strip->setBrightness(0);
      isOn = false;
      needsRefresh = true;
    }
    void on(){
      strip->setBrightness(brightness);
      isOn = true;
      needsRefresh = true;
    }
    void setBrightness(){
      strip->setBrightness(brightness);
    }
    void toggle(){
      if(isOn){
        off();
      }else{
        on();
      }
    }
    
 private:
  Adafruit_NeoPixel *strip;
  bool needsRefresh = true;
  bool isOn = false;
  int brightness = 200;
  int r,g,b,w = 0;
  void (LEDController::*internalUpdate)(void) = &LEDController::colorUpdate;
};
#endif
