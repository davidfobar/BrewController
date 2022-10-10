#ifndef BREW_STAND_CLASS_h
#define BREW_STAND_CLASS_h

//#define FULL_AUTOMATION

#include "Arduino.h"
#include "./BinaryOutputClass.h"
#include "./LiquidFlowSensorClass.h"
#include "./TemperatureClass.h"
#include "./RotaryInputClass.h"
#include "./PidRelayControllerClass.h"
#include "./BrewTimerClass.h"
#include "./constants.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

const char LOG_FILE_NAME[] = "brewStand.log";

//adjustable settings
const int GRAIN_ABSORBPTION = 0;
const int KETTLE_DEAD_SPACE = 1;
const int HLT_MLT_TRANSFER_LOSE = 2;
const int MLT_BK_TRANSFER_LOSE = 3;
const int BOIL_OFF_RATE = 4;
const int GRAIN_TEMP_ABS_FACTOR = 5;
const int SPARGE_TRANS_TEMP_LOSE = 6;
const int BK_P = 7;
const int BK_I = 8;
const int BK_D = 9;
const int RIMS_P = 10;
const int RIMS_I = 11;
const int RIMS_D = 12;
const int HLT_P = 13;
const int HLT_I = 14;
const int HLT_D = 15;
const int BK_SETPOINT = 16;
const int RIMS_SETPOINT = 17;
const int HLT_SETPOINT = 18;

const int GRAIN_ABSORBPTION_ADDR = 0;
const int KETTLE_DEAD_SPACE_ADDR = 4;
const int HLT_MLT_TRANSFER_LOSE_ADDR = 8;
const int MLT_BK_TRANSFER_LOSE_ADDR = 12;
const int BOIL_OFF_RATE_ADDR = 16;
const int GRAIN_TEMP_ABS_FACTOR_ADDR = 20;
const int SPARGE_TRANS_TEMP_LOSE_ADDR = 24;
const int BK_P_ADDR = 28;
const int BK_I_ADDR = 32;
const int BK_D_ADDR = 36;
const int RIMS_P_ADDR = 40;
const int RIMS_I_ADDR = 44;
const int RIMS_D_ADDR = 48;
const int HLT_P_ADDR = 52;
const int HLT_I_ADDR = 56;
const int HLT_D_ADDR = 60;

const int HLT_DIAMETER = 20;

const int BOIL_TIMER_START_TEMP = 208;
const double BOIL_TEMP = 212;
const int SETTLE_TIME = 1;
const double MIN_KETTLE_VOL = 5.0;

class BeerReciepeClass;

class BrewStandClass{
  private:
    BinaryOutputClass hltHeater, boilCoil, rimsHeater;
    BinaryOutputClass wortPump, waterPump, waterValve;
    OneWire oneWire;
    DallasTemperature tempProbeBus;
    TemperatureClass rimsTemp, bkTemp, hltTemp;
    LiquidFlowSensorClass rimsFlowSensor, waterInSensor;
    PidRelayControllerClass hltHeatPID;
    PidRelayControllerClass boilCoilPID;
    PidRelayControllerClass rimsHeatPID;
    BrewTimerClass brewTimer;

    BeerReciepeClass *thisReciepe;

    double bkSetpoint;
    double rimsSetpoint;
    double hltSetpoint;

    bool hltTempForNextStep;
    bool bkTempForNextStep;
    bool monitorWaterFlow;
    int brewingStage;

    bool mashingGrains;
    bool mashingout;
    bool collectingStrike;
    bool collectingSparge;
    bool heatingStrike;
    bool heatingSparge;
    bool transferingFirstRunnings;

    double grainAbsorption;
    double kettleDeadSpace;
    double mltToBkTransferLoss;
    double hltToMltTransferLoss;
    double boilOffRate;
    double hltDiameter;
    double grainTempAbsorbtionFactor;
    double spargeTempLoss;
    int pumpSanitizeTime;

    void collectStrikeWater();
    void heatStrikeWater();
    void transferStrikeToMLT();
    void doughIn();
    void collectSpargeWater();
    void heatSpargeWater();
    void mashGrains();
    void mashoutGrains();
    void transferFirstRunnings();
    void transferSpargeToMLT();
    void recircSparge();
    void transferSecondRunnings();
    void startBoil();
    void boil();
    void sanatizeWortPump();
    void chillWort();
    void transferWortToFermenter();
    void addHops(int hopperNum);
    void loadBrewStandSettings();

    bool logThisIteration;
    unsigned long datalogTimeStamp;
    unsigned long datalogPeriod;

  public:
    BrewStandClass();
    void setUp();
    void update();
    void cancelAll();
    void startBrewing(BeerReciepeClass *inThisReciepe);
    void cleanBrewStand();
    void nextStep();
    void adjustBoilIntensity(int dir);
    int getBrewingStage();
    void enableManualControl();
    void stopManualControl();

    void stopBoilCoilPID();
    void stopRimsPID();
    void stopHltPID();

    int getBkSetpoint();
    int getBkDutyCycle();
    int getBkTemp();
    int getRimsSetpoint();
    int getRimsDutyCycle();
    int getRimsTemp();
    int getHltSetpoint();
    int getHltDutyCycle();
    int getHltTemp();
    bool rimsActive();
    int checkRimsFlow();
    int getNextTimer();

    void changeSetting(int setting, int dir);
    double getSetting(int setting);
    void saveBrewStandSettings();
    void printTimers();
};

#endif
