#include "BinaryInputClass.h"

BinaryInputClass::BinaryInputClass(int inPin){
  pin = inPin;
  pinMode(pin, INPUT);
  state = digitalRead(pin);
  debouncing = false;
}

void BinaryInputClass::updateStatus(){
  if(!debouncing){
    bool currentState = digitalRead(pin);
    if(currentState != state){
      debouncing = true;
      changeTime = millis();
    }
  }
  else if(millis() - changeTime > DEBOUNCE_TIME){
    debouncing = false;
    state = digitalRead(pin);
  }
}

bool BinaryInputClass::getState(){
  return state;
}
