#include "TimerNodeClass.h"
//#include "../BrewStandClass/BrewStandClass.h"

TimerNodeClass::TimerNodeClass(int inEndTimeSec, int callBack){
  endTimeSec = inEndTimeSec;
  callBackItem = callBack;
}

int TimerNodeClass::isExpired(int &curSec){
  int result = NO_UPDATE;
  if(curSec >= endTimeSec){
    result = callBackItem;
  }
  return result;
}

int TimerNodeClass::getTimerEvent(){
  return callBackItem;
}

int TimerNodeClass::getSecRemaining(int &curSec){
  return endTimeSec - curSec;
}

TimerNodeClass* TimerNodeClass::getPrev(){
  return prevTimerNode;
}

TimerNodeClass* TimerNodeClass::getNext(){
  return nextTimerNode;
}

void TimerNodeClass::setPrevAndNext(TimerNodeClass *prev, TimerNodeClass *next){
  prevTimerNode = prev;
  nextTimerNode = next;

  if(prevTimerNode != NULL) prevTimerNode->nextTimerNode = this;
  if(nextTimerNode != NULL) nextTimerNode->prevTimerNode = this;
}

void TimerNodeClass::setPrev(TimerNodeClass *prev){
  prevTimerNode = prev;
}

void TimerNodeClass::setNext(TimerNodeClass *next){
  nextTimerNode = next;
}

int TimerNodeClass::getEndTime(){
  return endTimeSec;
}
