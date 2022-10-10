#include "DataLoggerClass.h"

DataLoggerClass::DataLoggerClass(int in_sdSelectPin, int in_sdDetectPin){
  sdSelectPin = in_sdSelectPin;
  sdDetectPin = in_sdDetectPin;
	pinMode(in_sdDetectPin,INPUT_PULLUP);
  if(digitalRead(in_sdDetectPin) == HIGH){
    sdCardPresent = true;
    SD.begin(sdSelectPin);
    if(SD.exists(LOG_FILE_NAME)){
      logFile  = SD.open(LOG_FILE_NAME, FILE_WRITE);
      logFile.println(F("Brewstand started, log file active"));
      logFile.close();
    }
  }
}

void DataLoggerClass::log(char *logString, int length){

}
