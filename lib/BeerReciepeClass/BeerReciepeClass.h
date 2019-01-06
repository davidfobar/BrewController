#ifndef BEER_RECIEPE_CLASS_h
#define BEER_RECIEPE_CLASS_h

#include "Arduino.h"
#include "../constants.h"

//reciepe page line options
const int GRAIN_BILL_LINE = 0;
const int MASH_TEMP_LINE = 1;
const int MASH_LENGTH_LINE = 2;
const int MASH_OUT_TEMP_LINE = 3;
const int BOIL_SCHEDULE_LINE = 4;
const int CHILL_TEMP_LINE = 5;
const int HOP_0_LINE = 7;
const int HOP_1_LINE = 8;
const int HOP_2_LINE = 9;
const int HOP_3_LINE = 10;

class BrewStandClass;

class BeerReciepeClass{
  private:
    double grainBillWeight = 1.0;
    double desiredVolume = 10.0;
    double mashRatio = 1.5;
    double mashTemp = 152;
    int tempAtStartup = 0;
    int mashLength = 2;
    int mashoutTemp = 170;
    int mashoutLength = 1;
    int spargeLength = 1;
    int boilLength = 5;
    int chillTemp = 75;
    int hopAddition[4] = {1, 2, 4, 0};

    int hopTimer[4] = {0};

    double volStrikeWater = 0;
    double volSpargeWater = 0;
    double strikeWaterTemp = 0;
    double spargeWaterTemp = 0;
    double volFirstRunning = 0;

    BrewStandClass *thisBrewStand;

  public:
    BeerReciepeClass(BrewStandClass &inThisBrewStand);
    void makeAdjustment(int lineNum, int dir);
    double getVolStrikeWater();
    double getVolSpargeWater();
    double getVolFirstRunning();
    double getStikeWaterTemp();
    double getStrikeTemp();
    double getSpargeTemp();
    double getGrainWeight();
    double getMashTemp();
    int getMashLenght();
    int getBoilLength();
    int* getHopSchedule();
    int getMashoutLenght();
    int getSpargeLenght();
    double getChillingTemp();
    double getMashoutTemp();
    void recalc();

};

#endif
