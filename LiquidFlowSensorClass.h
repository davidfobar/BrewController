#ifndef LIQUID_FLOW_SENSOR_CLASS_h
#define LIQUID_FLOW_SENSOR_CLASS_h

#include "Arduino.h"
#include "./constants.h"

const int TOO_SLOW = 0;
const int JUST_RIGHT = 1;
const int TOO_FAST = 2;
const double TOO_SLOW_FLOW_THRESHOLD = 0.1;
const double TOO_FAST_FLOW_THRESHOLD = 1.4; // gallons per minute
const int PULSE_TIMEOUT = 500;
const int NUM_TO_SUM = 4;

class LiquidFlowSensorClass{
  private:
    unsigned long prevPulseTime;
    int pulseCount;
    double flowRate;
    double flowRates[NUM_TO_SUM] = {0};
    double sum = 0;
    int oldestVal = 0;
    void _int_updateFlowCount();
    int prevState;
    int pin;
    int pulsesToCollect = 0;
    int pulsesInterated = 0;
    bool integrating = false;

  public:
    LiquidFlowSensorClass(int inPin);
    void updateFlowRate();
    bool isFlowing();
    int checkFlowRate();
    double getFlowRate();

    bool collectionComplete();
    void collectWater(double volume);

};

#endif
