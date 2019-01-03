#ifndef BREW_TIMER_CLASS_h
#define BREW_TIMER_CLASS_h

#include "Arduino.h"
#include "../constants.h"
#include "TimerNodeClass.h"

class BrewTimerClass{
  private:
    int curSec;
    unsigned long prevSecTic;
    TimerNodeClass *head = NULL;
    TimerNodeClass *tail = NULL;

  public:
    BrewTimerClass();
    void addTimer(int length, int callBack);
    int update();
    int getNextTimerInSec();
    void cancelTimer(int callBack);
    void clearAll();
    void printTimers();
};

#endif
