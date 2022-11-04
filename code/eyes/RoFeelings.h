#ifndef RoFeelings_h
#define RoFeelings_h
#include <Arduino.h>

class RoFeelings {
  public:
    RoFeelings(int SCREEN_WIDTH, int SCREEN_HEIGHT, int frames_delay);
    void begin();
    void blink();
  private:
    int _frames_delay;
    int _SCREEN_WIDTH;
    int _SCREEN_HEIGHT;
    
};
// code goes here
#endif