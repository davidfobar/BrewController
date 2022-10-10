#ifndef ROTARY_INPUT_CLASS_h
#define ROTARY_INPUT_CLASS_h

#include "Arduino.h"
#include <Rotary.h>
//#include <TimerOne.h>
//#include <Ticker.h>
#include "./constants.h"

const unsigned long TIMER_LENGTH = 2; //ms
const int ROTARY_DEBOUNCE_TIME = 10; //ms
const int LONG_PRESS_THRES = 400; //ms
const int SHORT_PRESS = 10;
const int LONG_PRESS = 11;
const int ROTATE_RIGHT = 0;
const int ROTATE_LEFT = 1;

class RotaryInputClass{
  private:
    enum rotaryStates {
      noRotaryUpdate,
      CW,
      CCW,
      shortPress,
      longPress
    } rotaryUpdate;

    unsigned long prevRotaryTime, prevButTime, firstButTime;
    int curButState, prevButState;
    Rotary rotaryInput;
    int pin;
    bool debouncing, enabled, firstLook;

  public:
    RotaryInputClass(int pin1, int pin2, int butPin);
    void enable();
    bool update();
    int getUserInput();
};

#endif
