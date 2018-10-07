#include "BrewTimerClass.h"
//#include "../BrewStandClass/BrewStandClass.h"

BrewTimerClass::BrewTimerClass(){
  curSec = 0;
  prevSecTic = millis();
  head = NULL;
  tail = NULL;
}

void BrewTimerClass::clearAll(){
  TimerNodeClass *temp = NULL;
  while(head != NULL){
    temp = head->getNext();
    delete head;
    head = temp;
  }
}

void BrewTimerClass::addTimer(int length, int callBack){
  int endTimeSec = curSec + length * SEC_PER_MIN;
  TimerNodeClass *newTimer = new TimerNodeClass(endTimeSec, callBack);
  if(head == NULL){
    head = newTimer;
    tail = newTimer;
    head->setPrevAndNext(NULL, NULL);
  }
  else if(newTimer->getEndTime() <= head->getEndTime()){
    newTimer->setPrevAndNext(NULL, head);
    head = newTimer;
  }
  else if(newTimer->getEndTime() >= tail->getEndTime()){
    newTimer->setPrevAndNext(tail, NULL);
    tail = newTimer;
  }
  else{
    TimerNodeClass *temp = head;
    while(newTimer->getEndTime() > temp->getEndTime()){
      temp = temp->getNext();
    }
    newTimer->setPrevAndNext(temp->getPrev(), temp);
  }
}

int BrewTimerClass::update(){
  int result = NO_UPDATE;

  int millisPassed = millis() - prevSecTic;
  if(millisPassed > MILLIS_PER_SECOND){
    curSec++;
    prevSecTic += MILLIS_PER_SECOND;
  }

  if(head != NULL){
    int tempResult = head->isExpired(curSec);
    if(tempResult != NO_UPDATE){
      result = tempResult;
      TimerNodeClass *temp = head->getNext();
      delete head;
      head = temp;
      temp->setPrev(NULL);
    }
  }
  return result;
}

void BrewTimerClass::cancelTimer(int callBack){
  TimerNodeClass *temp = head;
  while(temp != NULL){
    TimerNodeClass *tempNext = temp->getNext();
    TimerNodeClass *tempPrev = temp->getPrev();

    if(temp->getTimerEvent() == callBack){
      tempNext->setPrev(tempPrev);
      tempPrev->setNext(tempNext);

      if(tempPrev == NULL) head = tempNext;
      if(tempNext == NULL) tail = tempPrev;

      delete temp;
    }
    temp = tempNext;
  }
}

int BrewTimerClass::getNextTimerInSec(){
  if(head != NULL) return head->getSecRemaining(curSec);
  else return 0;
}

void BrewTimerClass::printTimers(){
  TimerNodeClass *temp = head;
  while(temp != NULL){
    temp = temp->getNext();
  }
}
