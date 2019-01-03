#include "BinaryOutputClass.h"

BinaryOutputClass::BinaryOutputClass(int outPin, int inLogicType){
  outputPin = outPin;
  logicType = inLogicType;

  pinMode(outputPin,OUTPUT);
  turnOff();
}

bool BinaryOutputClass::getState(){
  return state == ON;
}

void BinaryOutputClass::toggle(){
  if(state == ON){
    turnOff();
  }
  else{
    turnOn();
  }
}

void BinaryOutputClass::turnOn(){
  state = ON;
  if(logicType == ACTIVE_HIGH){
    digitalWrite(outputPin, ON);
  }
  else{
    digitalWrite(outputPin, OFF);
  }
}

void BinaryOutputClass::turnOff(){
  state = OFF;
  if(logicType == ACTIVE_HIGH){
    digitalWrite(outputPin, OFF);
  }
  else{
    digitalWrite(outputPin, ON);
  }
}
