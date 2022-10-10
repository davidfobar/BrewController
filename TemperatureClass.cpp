#include "TemperatureClass.h"

void TemperatureClass::linkProbe(DeviceAddress &inProbeAddr,
                                DallasTemperature &probeNetwork){
  probeAddr = &inProbeAddr;
  oneWireBus = &probeNetwork;
  oneWireBus->setWaitForConversion(false);
}

double TemperatureClass::getTemperature(){
  return temperature;
}

void TemperatureClass::updateTemp(){
  if(millis() - conversionStartTime > CONVERSION_TIME){
    temperature = oneWireBus->getTempF(*probeAddr);
    oneWireBus->requestTemperaturesByAddress(*probeAddr);
    conversionStartTime = millis();
  }
}
