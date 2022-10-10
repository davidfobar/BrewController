#include "./BeerReciepeClass.h"
#include "./BrewStandClass.h"

void BeerReciepeClass::makeAdjustment(int lineNum, int dir){
  if(lineNum == GRAIN_BILL_LINE){
    if(dir == UP) grainBillWeight += 0.1;
    else grainBillWeight -= 0.1;
  }
  if(lineNum == MASH_TEMP_LINE){
    if(dir == UP) mashTemp += 1;
    else mashTemp -= 1;
  }
  if(lineNum == MASH_LENGTH_LINE){
    if(dir == UP) mashLength += 1;
    else mashLength -= 1;
  }
  if(lineNum == MASH_OUT_TEMP_LINE){
    if(dir == UP) mashoutTemp += 1;
    else mashoutTemp -= 1;
  }
  if(lineNum == BOIL_SCHEDULE_LINE){
    if(dir == UP) boilLength += 1;
    else boilLength -= 1;
  }
  if(lineNum == CHILL_TEMP_LINE){
    if(dir == UP) chillTemp += 1;
    else chillTemp -= 1;
  }
  if(lineNum == HOP_0_LINE){
    if(dir == UP) hopAddition[0] += 1;
    else if(hopAddition[0] > 0)  hopAddition[0] -= 1;
  }
  if(lineNum == HOP_1_LINE){
    if(dir == UP) hopAddition[1] += 1;
    else if(hopAddition[1] > 0)  hopAddition[1] -= 1;
  }
  if(lineNum == HOP_2_LINE){
    if(dir == UP) hopAddition[2] += 1;
    else if(hopAddition[2] > 0) hopAddition[2] -= 1;
  }
  if(lineNum == HOP_3_LINE){
    if(dir == UP) hopAddition[3] += 1;
    else if(hopAddition[3] > 0)  hopAddition[3] -= 1;
  }
}

BeerReciepeClass::BeerReciepeClass(BrewStandClass &inThisBrewStand){
  thisBrewStand = &inThisBrewStand;
  tempAtStartup = 65;//thisBrewStand->getRimsTemp();
  recalc();
}

void BeerReciepeClass::recalc(){
  double gainTempAbsorbtion = thisBrewStand->getSetting(GRAIN_TEMP_ABS_FACTOR);
  strikeWaterTemp = (gainTempAbsorbtion / mashRatio) *
    (mashTemp - tempAtStartup) + mashTemp +
    thisBrewStand->getSetting(SPARGE_TRANS_TEMP_LOSE);
  delay(200);

  spargeWaterTemp = mashoutTemp +
    thisBrewStand->getSetting(SPARGE_TRANS_TEMP_LOSE);

  double hltToMltLoss = thisBrewStand->getSetting(HLT_MLT_TRANSFER_LOSE);
  double mltToBkLoss = thisBrewStand->getSetting(MLT_BK_TRANSFER_LOSE);
  double kettleDeadSpace = thisBrewStand->getSetting(KETTLE_DEAD_SPACE);
  double grainAbsorption = grainBillWeight * thisBrewStand->getSetting(GRAIN_ABSORBPTION);
  double boilOff = thisBrewStand->getSetting(BOIL_OFF_RATE) * boilLength / 60;

  volStrikeWater = (grainBillWeight * mashRatio)/4 + hltToMltLoss;

  volFirstRunning = volStrikeWater - mltToBkLoss - grainAbsorption;

  volSpargeWater = desiredVolume + kettleDeadSpace + grainAbsorption + boilOff +
    hltToMltLoss + mltToBkLoss - (grainBillWeight * mashRatio)/4;
}

double BeerReciepeClass::getVolStrikeWater(){
  return volStrikeWater;
}

double BeerReciepeClass::getVolSpargeWater(){
  return volSpargeWater;
}

double BeerReciepeClass::getVolFirstRunning(){
  return volFirstRunning;
}

double BeerReciepeClass::getSpargeTemp(){;
  return spargeWaterTemp;
}

int BeerReciepeClass::getSpargeLenght(){
  return spargeLength;
}

double BeerReciepeClass::getStrikeTemp(){
  return strikeWaterTemp;
}

double BeerReciepeClass::getGrainWeight(){
  return grainBillWeight;
}

double BeerReciepeClass::getMashTemp(){
  return mashTemp;
}

int BeerReciepeClass::getMashLenght(){
  return mashLength;
}

int BeerReciepeClass::getBoilLength(){
  return boilLength;
}

int* BeerReciepeClass::getHopSchedule(){
  for(int i=0; i<4; i++){
    hopTimer[i] = boilLength - hopAddition[i];
  }
  return hopTimer;
}

int BeerReciepeClass::getMashoutLenght(){
  return mashoutLength;
}

double BeerReciepeClass::getChillingTemp(){
  return chillTemp;
}

double BeerReciepeClass::getMashoutTemp(){
  return mashoutTemp;
}
