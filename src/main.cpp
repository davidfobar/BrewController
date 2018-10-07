
#include <Arduino.h>
#include "../BrewStandClass/BrewStandClass.h"
#include "../BeerReciepeClass/BeerReciepeClass.h"
#include "../UserInterfaceClass/UserInterfaceClass.h"

#include <ApplicationMonitor.h>

//Watchdog::CApplicationMonitor ApplicationMonitor(600);

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

  //ApplicationMonitor.Dump(Serial);
  //ApplicationMonitor.EnableWatchdog(Watchdog::CApplicationMonitor::Timeout_120ms);

  while(true){
    //ApplicationMonitor.IAmAlive();
    //ApplicationMonitor.SetData(0);
    loopCounter++;
    ui.update();
    //ApplicationMonitor.SetData(1);
    thisBrewStand.update();
    //ApplicationMonitor.SetData(2);
    ui.refresh(loopCounter);
  }
}

void loop(){}
