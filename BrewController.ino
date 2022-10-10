
#include <Arduino.h>
#include "./BrewStandClass.h"
#include "./BeerReciepeClass.h"
#include "./UserInterfaceClass.h"

void setup(){
  if(DEBUGGING){
    Serial.begin(9600);
    Serial.println("ready");
  }
  pinMode(ALARM_PIN, OUTPUT);
  digitalWrite(ALARM_PIN, LOW);

  BrewStandClass thisBrewStand;
  BeerReciepeClass thisReciepe(thisBrewStand);
  UserInterfaceClass ui(thisBrewStand, thisReciepe);

  thisBrewStand.setUp();
  int loopCounter = 0;

  while(true){
    loopCounter++;
    ui.update();
    thisBrewStand.update();
    ui.refresh(loopCounter);
  }
}

void loop(){}
