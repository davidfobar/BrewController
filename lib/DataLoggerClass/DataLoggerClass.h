#ifndef DATA_LOGGER_CLASS_h
#define DATA_LOGGER_CLASS_h

#include "SPI.h"
#include "SD.h"

const char LOG_FILE_NAME[] = "brewStand.log";

class DataLoggerClass{
  private:
    File logFile;
    bool sdCardPresent = false;
    int sdSelectPin = 0;
    int sdDetectPin = 0;
  public:
    DataLoggerClass(int in_sdSelectPin, int in_sdDetectPin);
    void log(char *logString, int length);
};

#endif
