#ifndef TEMPERATURE_CLASS_h
#define TEMPERATURE_CLASS_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include "./constants.h"

const int CONVERSION_TIME = 380;
const int TEMP_PROBE_RESOLUTION = 11; //bits

class TemperatureClass{
  private:
    DeviceAddress *probeAddr;
    DallasTemperature *oneWireBus;
    double temperature = 0;
    unsigned long conversionStartTime;

  public:

    void linkProbe(DeviceAddress &inProbeAddr, DallasTemperature &probeNetwork);
    double getTemperature();
    void updateTemp();
};

#endif
