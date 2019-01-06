#include "BrewStandClass.h"
#include "../BeerReciepeClass/BeerReciepeClass.h"

BrewStandClass::BrewStandClass() :
                   hltHeater(HLT_HEAT_PIN),
                   boilCoil(BOIL_COIL_PIN),
                   rimsHeater(RIMS_HEATER_PIN),
                   wortPump(WORT_PUMP_PIN),
                   waterPump(WATER_PUMP_PIN),
                   waterValve(WATER_VALVE_PIN),
                   oneWire(ONE_WIRE_BUS),
                   tempProbeBus(&oneWire),
                   rimsFlowSensor(RIMS_FLOW_INPUT_PIN),
                   waterInSensor(WATER_IN_INPUT_PIN),
                   hltHeatPID(hltTemp, 500, 0, 0, 1000, hltHeater),
                   boilCoilPID(bkTemp, 500, 0, 0, 1000, boilCoil),
                   rimsHeatPID(rimsTemp, 500, 2, 8, 1000, rimsHeater){
  bkSetpoint = 0;
  rimsSetpoint = 0;
  hltSetpoint = 0;

  hltTempForNextStep = false;
  bkTempForNextStep = false;
  monitorWaterFlow = false;
  brewingStage = NOT_BREWING;

  mashingGrains = false;
  mashingout = false;
  collectingStrike = false;
  collectingSparge = false;
  heatingStrike = false;
  heatingSparge = false;
  transferingFirstRunnings = false;

  grainAbsorption = 0.08;
  kettleDeadSpace = 0.375;
  mltToBkTransferLoss = 0.1;
  hltToMltTransferLoss = 0.1;
  boilOffRate = 1.2;
  hltDiameter = 12.75;
  grainTempAbsorbtionFactor = 0.2;
  spargeTempLoss = 1.0;
  pumpSanitizeTime = 1;
  logThisIteration = false;
  datalogPeriod = 1000;

  cancelAll();
}

void BrewStandClass::setUp(){
  tempProbeBus.setResolution(TEMP_PROBE_RESOLUTION);
  DeviceAddress BK_TEMP_PROBE_ADDR = {0x28, 0xFF, 0xD9, 0x69, 0x65, 0x15, 0x01, 0x3B};
  DeviceAddress RT_TEMP_PROBE_ADDR = {0x28, 0xFF, 0xA9, 0x6A, 0xA1, 0x16, 0x04, 0x79};
  DeviceAddress HLT_TEMP_PROBE_ADDR = {0x28, 0xFF, 0x06, 0x30, 0x65, 0x15, 0x01, 0x41};
  bkTemp.linkProbe(BK_TEMP_PROBE_ADDR, tempProbeBus);
  rimsTemp.linkProbe(RT_TEMP_PROBE_ADDR, tempProbeBus);
  hltTemp.linkProbe(HLT_TEMP_PROBE_ADDR, tempProbeBus);
  tempProbeBus.begin();
  tempProbeBus.requestTemperatures();

  loadBrewStandSettings();
}

void BrewStandClass::loadBrewStandSettings(){
  EEPROM.get(GRAIN_ABSORBPTION_ADDR, grainAbsorption);
  EEPROM.get(KETTLE_DEAD_SPACE_ADDR, kettleDeadSpace);
  EEPROM.get(HLT_MLT_TRANSFER_LOSE_ADDR, hltToMltTransferLoss);
  EEPROM.get(MLT_BK_TRANSFER_LOSE_ADDR, mltToBkTransferLoss);
  EEPROM.get(BOIL_OFF_RATE_ADDR, boilOffRate);
  EEPROM.get(GRAIN_TEMP_ABS_FACTOR_ADDR, grainTempAbsorbtionFactor);
  EEPROM.get(SPARGE_TRANS_TEMP_LOSE_ADDR, spargeTempLoss);

  double Kp, Ki, Kd;
  EEPROM.get(BK_P_ADDR, Kp);
  EEPROM.get(BK_I_ADDR, Ki);
  EEPROM.get(BK_D_ADDR, Kd);
  boilCoilPID.changeTunings(Kp, Ki, Kd);

  EEPROM.get(RIMS_P_ADDR, Kp);
  EEPROM.get(RIMS_I_ADDR, Ki);
  EEPROM.get(RIMS_D_ADDR, Kd);
  rimsHeatPID.changeTunings(Kp, Ki, Kd);

  EEPROM.get(HLT_P_ADDR, Kp);
  EEPROM.get(HLT_I_ADDR, Ki);
  EEPROM.get(HLT_D_ADDR, Kd);
  hltHeatPID.changeTunings(Kp, Ki, Kd);
}

void BrewStandClass::saveBrewStandSettings(){
  EEPROM.put(GRAIN_ABSORBPTION_ADDR, grainAbsorption);
  EEPROM.put(KETTLE_DEAD_SPACE_ADDR, kettleDeadSpace);
  EEPROM.put(HLT_MLT_TRANSFER_LOSE_ADDR, hltToMltTransferLoss);
  EEPROM.put(MLT_BK_TRANSFER_LOSE_ADDR, mltToBkTransferLoss);
  EEPROM.put(BOIL_OFF_RATE_ADDR, boilOffRate);
  EEPROM.put(GRAIN_TEMP_ABS_FACTOR_ADDR, grainTempAbsorbtionFactor);
  EEPROM.put(SPARGE_TRANS_TEMP_LOSE_ADDR, spargeTempLoss);
  EEPROM.put(BK_P_ADDR, boilCoilPID.getKp());
  EEPROM.put(BK_I_ADDR, boilCoilPID.getKi());
  EEPROM.put(BK_D_ADDR, boilCoilPID.getKd());
  EEPROM.put(RIMS_P_ADDR, rimsHeatPID.getKp());
  EEPROM.put(RIMS_I_ADDR, rimsHeatPID.getKi());
  EEPROM.put(RIMS_D_ADDR, rimsHeatPID.getKd());
  EEPROM.put(HLT_P_ADDR, hltHeatPID.getKp());
  EEPROM.put(HLT_I_ADDR, hltHeatPID.getKi());
  EEPROM.put(HLT_D_ADDR, hltHeatPID.getKd());
}

void BrewStandClass::update(){
  rimsTemp.updateTemp();
  bkTemp.updateTemp();
  hltTemp.updateTemp();

  rimsFlowSensor.updateFlowRate();
  waterInSensor.updateFlowRate();

  hltHeatPID.checkPIDandOperateRelay();
  boilCoilPID.checkPIDandOperateRelay();
  int flowRateLevel = checkRimsFlow();
  if(flowRateLevel == TOO_SLOW) rimsHeatPID.standby();
  else rimsHeatPID.checkPIDandOperateRelay();

  if(brewingStage != MANUAL_CONTROL){
    //if a timer expires, the function attached to it is called at that time
    int nextTimerAction = brewTimer.update();
    if(nextTimerAction != NO_UPDATE){
      if(nextTimerAction == TRANSFER_FIRST_RUNNING)        transferFirstRunnings();
      else if(nextTimerAction == TRANSFER_SECOND_RUNNINGS) transferSecondRunnings();
      else if(nextTimerAction == MASHOUT_GRAINS)           mashoutGrains();
      else if(nextTimerAction == SANITIZE_PUMP)            sanatizeWortPump();
      else if(nextTimerAction == CHILL)                    chillWort();
      else if(nextTimerAction == ADD_HOP0)                 addHops(ADD_HOP0);
      else if(nextTimerAction == ADD_HOP1)                 addHops(ADD_HOP1);
      else if(nextTimerAction == ADD_HOP2)                 addHops(ADD_HOP2);
      else if(nextTimerAction == ADD_HOP3)                 addHops(ADD_HOP3);
      else if(nextTimerAction == TRANSFER_TO_FERMENTER)    transferWortToFermenter();
    }

    if(monitorWaterFlow && waterInSensor.collectionComplete()){
      if(collectingStrike){
        waterValve.turnOff();
        heatStrikeWater();
      }
      else if(collectingSparge){
        waterValve.turnOff();
        heatSpargeWater();
      }
    }

    if(hltTempForNextStep){
      if(int(hltTemp.getTemperature()) >= int(hltSetpoint)){
        if(heatingStrike) transferStrikeToMLT();
        else if(heatingSparge) transferSpargeToMLT();
      }
    }

    if(bkTempForNextStep){
      if(brewingStage == STARTING_BOIL){
        if(int(bkTemp.getTemperature()) >= BOIL_TIMER_START_TEMP){
          boil();
        }
      }
      else if(brewingStage == CHILL){
        if(int(bkTemp.getTemperature()) <= thisReciepe->getChillingTemp()){
          brewTimer.addTimer(SETTLE_TIME, TRANSFER_TO_FERMENTER);
          brewingStage = SETTLING;
        }
      }
    }
  }
}

void BrewStandClass::cancelAll(){
  if(brewingStage != MANUAL_CONTROL){
    brewingStage = NOT_BREWING;
    waterPump.turnOff();
    wortPump.turnOff();
  }
  waterValve.turnOff();
  bkSetpoint = 0;
  rimsSetpoint = 0;
  hltSetpoint = 0;
  hltTempForNextStep = false;
  bkTempForNextStep = false;
  monitorWaterFlow = false;
  mashingGrains = false;
  mashingout = false;
  collectingStrike = false;
  collectingSparge = false;
  heatingStrike = false;
  heatingSparge = false;
  transferingFirstRunnings = false;
  boilCoilPID.disable();
  rimsHeatPID.disable();
  hltHeatPID.disable();
  brewTimer.clearAll();
}

void BrewStandClass::stopBoilCoilPID(){
  boilCoilPID.setDCtoZero();
}

void BrewStandClass::stopRimsPID(){
  rimsSetpoint = 0;
  rimsHeatPID.disable();
}

void BrewStandClass::stopHltPID(){
  hltSetpoint = 0;
  hltHeatPID.disable();
}

void BrewStandClass::enableManualControl(){
  brewingStage = MANUAL_CONTROL;
  boilCoilPID.enableManual();
  waterPump.turnOn();
  wortPump.turnOn();
}

void BrewStandClass::stopManualControl(){
  brewingStage = NOT_BREWING;
  boilCoilPID.disable();
  waterPump.turnOff();
  wortPump.turnOff();
}

void BrewStandClass::nextStep(){
  if(brewingStage == COLLECTING_STRIKE)                 heatStrikeWater();
  else if(brewingStage == HEATING_STRIKE)               transferStrikeToMLT();
  else if(brewingStage == TRANSFER_STRIKE)              doughIn();
  else if(brewingStage == DOUGH_IN)                     mashGrains();
  else if(brewingStage == MASH_WHILE_COLLECTING_SPARGE) heatSpargeWater();
  else if(brewingStage == MASH_WHILE_HEATING_SPARGE)    brewingStage = MASH_GRAINS;
  else if(brewingStage == MASH_GRAINS)                  brewingStage = MASHOUT_GRAINS;
  else if(brewingStage == MASHOUT_GRAINS)               transferFirstRunnings();
  else if(brewingStage == TRANSFER_FIRST_RUNNING)       transferSpargeToMLT();
  else if(brewingStage == TRANSFER_SPARGE)              recircSparge();
  else if(brewingStage == SPARGE_GRAINS)                transferSecondRunnings();
  else if(brewingStage == TRANSFER_SECOND_RUNNINGS)     startBoil();
  else if(brewingStage == STARTING_BOIL)                boil();
  else if(brewingStage == BOIL)                         chillWort();
  else if(brewingStage == CHILL)                        brewingStage = SETTLING;
  else if(brewingStage == SETTLING)                     transferWortToFermenter();
  else if(brewingStage == TRANSFER_TO_FERMENTER)        cleanBrewStand();
  else if(brewingStage == CLEANING_BREWSTAND)           cancelAll();
}

void BrewStandClass::startBrewing(BeerReciepeClass *inThisReciepe){
  thisReciepe = inThisReciepe;
  collectStrikeWater();
}

void BrewStandClass::collectStrikeWater(){
  brewingStage = COLLECTING_STRIKE;
  collectingStrike = true;
  monitorWaterFlow = true;
  waterValve.turnOn();
  waterInSensor.collectWater(thisReciepe->getVolStrikeWater());
  //monitor water in for stirke volume
}

void BrewStandClass::heatStrikeWater(){
  brewingStage = HEATING_STRIKE;
  waterValve.turnOff();
  heatingStrike = true;
  collectingStrike = false;
  monitorWaterFlow = false;
  //set water pump out to HLT
  waterPump.turnOn();
  hltSetpoint = thisReciepe->getStrikeTemp();
  hltHeatPID.enableWithSetpoint(hltSetpoint);
  hltTempForNextStep = true;
}

void BrewStandClass::transferStrikeToMLT(){
  brewingStage = TRANSFER_STRIKE;
  hltTempForNextStep = false;
  heatingStrike = false;
  //set water pump out to MLT
  hltSetpoint = 0;
  hltHeatPID.disable();
  waterPump.turnOn();
}

void BrewStandClass::doughIn(){
  brewingStage = DOUGH_IN;
  waterPump.turnOff();
}

void BrewStandClass::mashGrains(){
  brewingStage = MASH_WHILE_COLLECTING_SPARGE;
  mashingGrains = true;
  //pump in mlt
  //pump out mlt
  wortPump.turnOn();
  rimsSetpoint = thisReciepe->getMashTemp();
  rimsHeatPID.enableWithSetpoint(rimsSetpoint);
  int timerLength = thisReciepe->getMashLenght() - thisReciepe->getMashoutLenght();
  brewTimer.addTimer(timerLength, MASHOUT_GRAINS);

  collectSpargeWater();
}

void BrewStandClass::mashoutGrains(){
  brewingStage = MASHOUT_GRAINS;
  mashingout = true;
  rimsSetpoint = thisReciepe->getMashoutTemp();
  rimsHeatPID.enableWithSetpoint(rimsSetpoint);
  int timerLength = thisReciepe->getMashoutLenght();
  brewTimer.addTimer(timerLength, TRANSFER_FIRST_RUNNING);
}

void BrewStandClass::collectSpargeWater(){
  monitorWaterFlow = true;
  collectingSparge = true;
  waterValve.turnOn();
  waterInSensor.collectWater(thisReciepe->getVolSpargeWater());
  //monitor water in for sparge volume
}

void BrewStandClass::heatSpargeWater(){
  brewingStage = MASH_WHILE_HEATING_SPARGE;
  waterValve.turnOff();
  collectingSparge = false;
  monitorWaterFlow = false;
  heatingSparge = true;
  //set water pump out to HLT
  waterPump.turnOn();
  hltSetpoint = thisReciepe->getSpargeTemp();
  hltHeatPID.enableWithSetpoint(hltSetpoint);
  hltTempForNextStep = true;

}

void BrewStandClass::transferFirstRunnings(){
  brewingStage = TRANSFER_FIRST_RUNNING;
  brewTimer.cancelTimer(MASHOUT_GRAINS);
  transferingFirstRunnings = true;
  rimsSetpoint = 0;
  rimsHeatPID.disable();
  //set rims out to BK

}

void BrewStandClass::transferSpargeToMLT(){
  brewingStage = TRANSFER_SPARGE;
  heatingSparge = false;
  brewTimer.cancelTimer(TRANSFER_FIRST_RUNNING);
  hltTempForNextStep = false;
  //set water pump out to MLT
  wortPump.turnOff();
  waterPump.turnOn();
  hltSetpoint = 0;
  hltHeatPID.disable();

}

void BrewStandClass::recircSparge(){
  brewingStage = SPARGE_GRAINS;
  waterPump.turnOff();
  //pump in mlt
  //pump out mlt
  wortPump.turnOn();
  rimsSetpoint = thisReciepe->getMashoutTemp();
  rimsHeatPID.enableWithSetpoint(rimsSetpoint);
  int timerLength = thisReciepe->getSpargeLenght();
  brewTimer.addTimer(timerLength, TRANSFER_SECOND_RUNNINGS);

  if(thisReciepe->getVolFirstRunning() > MIN_KETTLE_VOL){
    bkSetpoint = 205;
    boilCoilPID.enableWithSetpoint(bkSetpoint);
  }
}

void BrewStandClass::transferSecondRunnings(){
  brewingStage = TRANSFER_SECOND_RUNNINGS;
  rimsSetpoint = 0;
  rimsHeatPID.disable();
  //pump out bk
}

void BrewStandClass::startBoil(){
  brewingStage = STARTING_BOIL;
  brewTimer.cancelTimer(TRANSFER_SECOND_RUNNINGS);
  wortPump.turnOff();
  //pump in bk
  //pump out bk
  bkSetpoint = 212;
  boilCoilPID.enableWithSetpoint(bkSetpoint);
  bkTempForNextStep = true;
}

void BrewStandClass::boil(){
  brewingStage = BOIL;
  bkTempForNextStep = false;
  boilCoilPID.enableManual();

  int *hopSchedule = thisReciepe->getHopSchedule();
  brewTimer.addTimer(hopSchedule[0], ADD_HOP0);
  brewTimer.addTimer(hopSchedule[1], ADD_HOP1);
  brewTimer.addTimer(hopSchedule[2], ADD_HOP2);
  brewTimer.addTimer(hopSchedule[3], ADD_HOP3);

  brewTimer.addTimer(thisReciepe->getBoilLength() - pumpSanitizeTime, SANITIZE_PUMP);
  brewTimer.addTimer(thisReciepe->getBoilLength(), CHILL);
}

void BrewStandClass::adjustBoilIntensity(int dir){
  boilCoilPID.adjustDC(dir);
}

void BrewStandClass::sanatizeWortPump(){
  wortPump.turnOn();
}

void BrewStandClass::chillWort(){
  brewingStage = CHILL;
  brewTimer.cancelTimer(ADD_HOP0);
  brewTimer.cancelTimer(ADD_HOP1);
  brewTimer.cancelTimer(ADD_HOP2);
  brewTimer.cancelTimer(ADD_HOP3);
  brewTimer.cancelTimer(SANITIZE_PUMP);
  brewTimer.cancelTimer(CHILL);
  wortPump.turnOn();
  bkSetpoint = thisReciepe->getChillingTemp();
  boilCoilPID.disable();
  //operate chilling water flow
  bkTempForNextStep = true;
}

void BrewStandClass::transferWortToFermenter(){
  brewingStage = TRANSFER_TO_FERMENTER;
  bkTempForNextStep = false;
  bkSetpoint = 0;
  wortPump.turnOff();
  //brewTimer.setCountDownTimer(SETTLE_TIME);
}

void BrewStandClass::addHops(int hopperNum){

}

void BrewStandClass::cleanBrewStand(){
  brewingStage = CLEANING_BREWSTAND;
  brewTimer.cancelTimer(TRANSFER_TO_FERMENTER);
}

int BrewStandClass::getBrewingStage(){
  return brewingStage;
}

int BrewStandClass::getNextTimer(){
  return brewTimer.getNextTimerInSec();
}

double BrewStandClass::getSetting(int setting){
  double val;
  if(setting == KETTLE_DEAD_SPACE){
    val = kettleDeadSpace;
  }
  else if(setting == GRAIN_ABSORBPTION){
    val = grainAbsorption;
  }
  else if(setting == MLT_BK_TRANSFER_LOSE){
    val = mltToBkTransferLoss;
  }
  else if(setting == HLT_MLT_TRANSFER_LOSE){
    val = hltToMltTransferLoss;
  }
  else if(setting == BOIL_OFF_RATE){
    val = boilOffRate;
  }
  else if(setting == GRAIN_TEMP_ABS_FACTOR){
    val = grainTempAbsorbtionFactor;
  }
  else if(setting == SPARGE_TRANS_TEMP_LOSE){
    val = spargeTempLoss;
  }
  else if(setting == HLT_P){
    val = hltHeatPID.getKp();
  }
  else if(setting == HLT_I){
    val = hltHeatPID.getKi();;
  }
  else if(setting == HLT_D){
    val = hltHeatPID.getKd();;
  }
  else if(setting == RIMS_P){
    val = rimsHeatPID.getKp();
  }
  else if(setting == RIMS_I){
    val = rimsHeatPID.getKi();
  }
  else if(setting == RIMS_D){
    val = rimsHeatPID.getKd();
  }
  else if(setting == BK_P){
    val = boilCoilPID.getKp();
  }
  else if(setting == BK_I){
    val = boilCoilPID.getKi();;
  }
  else if(setting == BK_D){
    val = boilCoilPID.getKd();;
  }
  else if(setting == HLT_DIAMETER){
    val = hltDiameter;
  }
  return val;
}

void BrewStandClass::changeSetting(int setting, int dir){
  if(setting == KETTLE_DEAD_SPACE){
    if(dir == UP) kettleDeadSpace += 0.01;
    else kettleDeadSpace -= 0.01;
  }
  else if(setting == GRAIN_ABSORBPTION){
    if(dir == UP) grainAbsorption += 0.01;
    else grainAbsorption -= 0.01;
  }
  else if(setting == MLT_BK_TRANSFER_LOSE){
    if(dir == UP) mltToBkTransferLoss += 0.01;
    else mltToBkTransferLoss -= 0.01;
  }
  else if(setting == HLT_MLT_TRANSFER_LOSE){
    if(dir == UP) hltToMltTransferLoss += 0.01;
    else hltToMltTransferLoss -= 0.01;
  }
  else if(setting == BOIL_OFF_RATE){
    if(dir == UP) boilOffRate += 0.1;
    else boilOffRate -= 0.1;
  }
  else if(setting == GRAIN_TEMP_ABS_FACTOR){
    if(dir == UP) grainTempAbsorbtionFactor += 0.1;
    else grainTempAbsorbtionFactor -= 0.1;
  }
  else if(setting == SPARGE_TRANS_TEMP_LOSE){
    if(dir == UP) spargeTempLoss += 0.1;
    else spargeTempLoss -= 0.1;
  }
  else if(setting == BK_P){
    double Kp = boilCoilPID.getKp();
    double Ki = boilCoilPID.getKi();
    double Kd = boilCoilPID.getKd();
    if(dir == UP) Kp += 10;
    else Kp -= 10;
    boilCoilPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == BK_I){
    double Kp = boilCoilPID.getKp();
    double Ki = boilCoilPID.getKi();
    double Kd = boilCoilPID.getKd();
    if(dir == UP) Ki += 1;
    else Ki -= 1;
    boilCoilPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == BK_D){
    double Kp = boilCoilPID.getKp();
    double Ki = boilCoilPID.getKi();
    double Kd = boilCoilPID.getKd();
    if(dir == UP) Kd += 1;
    else Kd -= 1;
    boilCoilPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == RIMS_P){
    double Kp = rimsHeatPID.getKp();
    double Ki = rimsHeatPID.getKi();
    double Kd = rimsHeatPID.getKd();
    if(dir == UP) Kp += 10;
    else Kp -= 10;
    rimsHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == RIMS_I){
    double Kp = rimsHeatPID.getKp();
    double Ki = rimsHeatPID.getKi();
    double Kd = rimsHeatPID.getKd();
    if(dir == UP) Ki += 1;
    else Ki -= 1;
    rimsHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == RIMS_D){
    double Kp = rimsHeatPID.getKp();
    double Ki = rimsHeatPID.getKi();
    double Kd = rimsHeatPID.getKd();
    if(dir == UP) Kd += 1;
    else Kd -= 1;
    rimsHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == HLT_P){
    double Kp = hltHeatPID.getKp();
    double Ki = hltHeatPID.getKi();
    double Kd = hltHeatPID.getKd();
    if(dir == UP) Kp += 10;
    else Kp -= 10;
    hltHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == HLT_I){
    double Kp = hltHeatPID.getKp();
    double Ki = hltHeatPID.getKi();
    double Kd = hltHeatPID.getKd();
    if(dir == UP) Ki += 1;
    else Ki -= 1;
    hltHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == HLT_D){
    double Kp = hltHeatPID.getKp();
    double Ki = hltHeatPID.getKi();
    double Kd = hltHeatPID.getKd();
    if(dir == UP) Kd += 1;
    else Kd -= 1;
    hltHeatPID.changeTunings(Kp, Ki, Kd);
  }
  else if(setting == BK_SETPOINT){
    if(dir == UP) bkSetpoint += 5;
    else bkSetpoint -= 1;
    boilCoilPID.enableWithSetpoint(bkSetpoint);
  }
  else if(setting == RIMS_SETPOINT){
    if(dir == UP) rimsSetpoint += 5;
    else rimsSetpoint -= 1;
    rimsHeatPID.enableWithSetpoint(rimsSetpoint);
  }
  else if(setting == HLT_SETPOINT){
    if(dir == UP) hltSetpoint += 5;
    else hltSetpoint -= 1;
    hltHeatPID.enableWithSetpoint(hltSetpoint);
  }
}
int BrewStandClass::getHltSetpoint(){
  return int(hltSetpoint);
}
int BrewStandClass::getHltTemp(){
  if(int(hltTemp.getTemperature() < 0)) return 0;
  else return int(hltTemp.getTemperature());
}
int BrewStandClass::getBkSetpoint(){
  return int(bkSetpoint);
}
int BrewStandClass::getBkDutyCycle(){
  return boilCoilPID.getDutyCycle();
}
int BrewStandClass::getRimsDutyCycle(){
  return rimsHeatPID.getDutyCycle();
}
int BrewStandClass::getHltDutyCycle(){
  return hltHeatPID.getDutyCycle();
}
int BrewStandClass::getBkTemp(){
  if(int(bkTemp.getTemperature() < 0)) return 0;
  else return int(bkTemp.getTemperature());
}
int BrewStandClass::getRimsSetpoint(){
  return int(rimsSetpoint);
}
int BrewStandClass::getRimsTemp(){
  if(int(rimsTemp.getTemperature() < 0)) return 0;
  else return int(rimsTemp.getTemperature());
}
bool BrewStandClass::rimsActive(){
  return rimsHeatPID.isActive();
}
int BrewStandClass::checkRimsFlow(){
  return rimsFlowSensor.checkFlowRate();
}

void BrewStandClass::printTimers(){
  brewTimer.printTimers();
}
