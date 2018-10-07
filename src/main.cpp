
#include <Arduino.h>
#include "../BrewStandClass/BrewStandClass.h"
#include "../BeerReciepeClass/BeerReciepeClass.h"
#include "../UserInterfaceClass/UserInterfaceClass.h"

void setup(){
  //Serial.begin(9600);
  //Serial.println("ready");

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
