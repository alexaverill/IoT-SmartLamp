#ifndef LEDCONTROLLER
#define LEDCONTROLLER
#include <functional>
#include <Adafruit_NeoPixel.h>

//int (*fcnPtr)(int){ &foo }; // Initialize fcnPtr with function foo
 //   (*fcnPtr)(5); // call function foo(5) through fcnPtr.
 //int (Data::*fp1) (float) = &Data::f; 
class LEDController{
  public:
    LEDController(Adafruit_NeoPixel *_strip, std::function<void (bool)> switchCallback,std::function<void (bool,int,int,int)> colorCallback,std::function<void (bool,int)> brightnessCallback){
      strip = _strip;     
      SwitchCallBack = switchCallback;
      ColorCallback = colorCallback;
      BrightnessCallback = brightnessCallback;
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
    void setColor(int _r, int _g, int _b){
      r = _r;
      g = _g;
      b = _b;
      w=0;
      internalUpdate =&LEDController::colorUpdate;
      needsRefresh = true;
      ColorCallback(isOn,r,g,b);
    }
    
    void colorUpdate(){
        Serial.println("Updating LEDs");
        Serial.println(r);
        Serial.println(g);
        Serial.println(b);
        strip->fill(strip->Color(r,g,b,w));
        strip->show();
        needsRefresh= false;
    }

    void off(){
      strip->fill(strip->Color(0,0,0,0));
      strip->show();
      isOn = false;
      SwitchCallBack(isOn);
      //needsRefresh = true;
    }
    void on(){
      if(isOn){return;}
      //strip->setBrightness(brightness);
      isOn = true;
      SwitchCallBack(isOn);
      needsRefresh = true;
    }
    void setBrightness(int _brightness){
      brightness= _brightness;
      strip->setBrightness(brightness);
      strip->show();
      BrightnessCallback(isOn,brightness);
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
  int r,g,b,w = 200;
  void (LEDController::*internalUpdate)(void) = &LEDController::colorUpdate;
  std::function<void (bool)> SwitchCallBack;
  std::function<void (bool,int,int,int)>ColorCallback;
  std::function<void (bool,int)>BrightnessCallback;
};
#endif
