#include "RotaryInputClass.h"

RotaryInputClass::RotaryInputClass(int pin1, int pin2, int butPin) : rotaryInput(pin1,pin2){
  pin = butPin;
  pinMode(pin, INPUT_PULLUP);
  rotaryUpdate = noRotaryUpdate;
  curButState = LOW;
  prevButState = LOW;
  debouncing = false;
  enabled = false;
  firstLook = true;
  prevRotaryTime = millis();
  prevButTime = millis();
}

void RotaryInputClass::enable(){
  enabled = true;
}

bool RotaryInputClass::update(){
  bool updateFound = false;

  if(enabled){
    //process rotary input
    if(millis() - prevRotaryTime > TIMER_LENGTH){
      prevRotaryTime = millis();
      unsigned char result = rotaryInput.process();
      if 		  (result == DIR_CW) 	{rotaryUpdate = CW;  updateFound = true;}
      else if (result == DIR_CCW) {rotaryUpdate = CCW; updateFound = true;}
    }

    //if no rotary input, process push button
    if(!updateFound){
      if(!debouncing){
        int currentButState = digitalRead(pin);
        if(currentButState != prevButState){
          debouncing = true;
          prevButTime = millis();
        }
      }
      else if(millis() - prevButTime > ROTARY_DEBOUNCE_TIME){
        debouncing = false;
        curButState = digitalRead(pin);

        //the button was just pressed
        if(prevButState == HIGH && curButState == LOW){
          firstButTime = millis();
        }
        //the button was just released
        else if(prevButState == LOW && curButState == HIGH){
          if(millis() - firstButTime > LONG_PRESS_THRES){
            rotaryUpdate = longPress;
          }
          else rotaryUpdate = shortPress;

          updateFound = true;
        }

        prevButState = curButState;
      }
    }
    if(updateFound && firstLook) {
      updateFound = false;
      firstLook = false;
      rotaryUpdate = noRotaryUpdate;
    }
  }

  return updateFound;
}

int RotaryInputClass::getUserInput(){
  int result;
  if(rotaryUpdate == CW) result = ROTATE_RIGHT;
  else if(rotaryUpdate == CCW) result = ROTATE_LEFT;
  else if(rotaryUpdate == shortPress) result = SHORT_PRESS;
  else if(rotaryUpdate == longPress) result = LONG_PRESS;

  rotaryUpdate = noRotaryUpdate;
  return result;
}
