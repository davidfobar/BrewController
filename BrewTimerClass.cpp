#include "BrewTimerClass.h"
//#include "../BrewStandClass/BrewStandClass.h"

BrewTimerClass::BrewTimerClass(){
  curSec = 0;
  prevSecTic = millis();
  head = NULL;
  tail = NULL;
}

void BrewTimerClass::clearAll(){
  if(DEBUGGING) Serial.println();
  if(DEBUGGING) Serial.println("  clearing timers");
  if(DEBUGGING) printTimers();
  TimerNodeClass *temp = NULL;
  while(head != NULL){
    temp = head->getNext();
    delete head;
    head = temp;
  }
  if(DEBUGGING) printTimers();
  if(DEBUGGING) Serial.println("  timer clearing complete");
}

void BrewTimerClass::addTimer(int length, int callBack){
  if(DEBUGGING) Serial.println();
  if(DEBUGGING) Serial.print("  adding timer with event and length ");
  if(DEBUGGING) Serial.print(callBack);
  if(DEBUGGING) Serial.print(" ");
  if(DEBUGGING) Serial.println(length);
  if(DEBUGGING) printTimers();
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
  if(DEBUGGING) printTimers();
  if(DEBUGGING) Serial.println("  timer adding complete");
}

int BrewTimerClass::update(){
  int result = NO_UPDATE;

  int millisPassed = millis() - prevSecTic;
  if(millisPassed > MILLIS_PER_SECOND){
    curSec++;
    prevSecTic += MILLIS_PER_SECOND;
    if(DEBUGGING) Serial.print(curSec);
    if(DEBUGGING) Serial.print("\r");
  }

  if(head != NULL){
    int tempResult = head->isExpired(curSec);
    if(tempResult != NO_UPDATE){
      if(DEBUGGING) Serial.println();
      if(DEBUGGING) Serial.println("  head expired");
      if(DEBUGGING) printTimers();
      result = tempResult;
      TimerNodeClass *temp = head->getNext();
      delete head;
      head = temp;
      if(temp != NULL) temp->setPrev(NULL);
      if(DEBUGGING) printTimers();
      if(DEBUGGING) Serial.println("  update complete");
    }
  }
  return result;
}

void BrewTimerClass::cancelTimer(int callBack){
  if(DEBUGGING) Serial.println();
  if(DEBUGGING) Serial.print("  attempting to cancel timer: ");
  if(DEBUGGING) Serial.println(callBack);
  if(DEBUGGING) printTimers();
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
  if(DEBUGGING) printTimers();
  if(DEBUGGING) Serial.println("  cancel timers complete");
}

int BrewTimerClass::getNextTimerInSec(){
  if(head != NULL) return head->getSecRemaining(curSec);
  else return 0;
}

void BrewTimerClass::printTimers(){
  TimerNodeClass *temp = head;
  if(DEBUGGING) Serial.print("    current timers: ");
  if(DEBUGGING && temp == NULL) Serial.print("no timers in quene");
  while(temp != NULL){
    if(DEBUGGING) Serial.print(temp->getTimerEvent());
    if(DEBUGGING) Serial.print(" ");
    temp = temp->getNext();
  }
  if(DEBUGGING) Serial.println();
}
