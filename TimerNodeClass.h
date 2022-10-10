#ifndef TIMER_NODE_CLASS_h
#define TIMER_NODE_CLASS_h

#include "Arduino.h"
#include "./constants.h"

class BrewStandClass;

class TimerNodeClass{
  private:
    int endTimeSec = 0;
    TimerNodeClass *prevTimerNode = NULL;
    TimerNodeClass *nextTimerNode = NULL;
    int callBackItem = 0;

  public:
    TimerNodeClass(int inEndTimeSec, int callBack);

    int isExpired(int &curSec);
    int getTimerEvent();
    TimerNodeClass* getPrev();
    TimerNodeClass* getNext();
    void setPrevAndNext(TimerNodeClass *prev, TimerNodeClass *next);
    void setPrev(TimerNodeClass *prev);
    void setNext(TimerNodeClass *next);
    int getSecRemaining(int &curSec);

    int getEndTime();
};

#endif
