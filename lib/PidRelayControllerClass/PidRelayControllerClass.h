#ifndef PID_RELAY_CONTROLLER_CLASS_h
#define PID_RELAY_CONTROLLER_CLASS_h

#include <PID_v1.h>
#include "Arduino.h"
#include "../BinaryOutputClass/BinaryOutputClass.h"
#include "../TemperatureClass/TemperatureClass.h"
#include "../constants.h"

class PidRelayControllerClass{
  private:
    bool manual = false;
    TemperatureClass *inputSource;
    double setpoint;
    double input;
    double Kp, Ki, Kd;
    double outputLength;
    int dutyCycle;
    unsigned int windowSize;
    unsigned long windowStartTime;
    bool PIDisActive;
    BinaryOutputClass *controlledRelay;
    PID _PID;

  public:
    void changeSetpointTo(double newSetpoint);

    void changeTunings(double inKp, double inKi, double inKd);

    void changeWindowSize(int inWindowSize);

    void checkPIDandOperateRelay();

    void standby();

    void enableWithSetpoint(double newSetpoint);

    void enableManual();

    void adjustDC(int dir);

    void setDCtoZero();

    int getDutyCycle();

    void disable();

    bool isActive();

    double getKp(){
      return Kp;
    }

    double getKi(){
      return Ki;
    }

    double getKd(){
      return Kd;
    }

    PidRelayControllerClass(TemperatureClass &source, double inKp, double inKi,
                            double inKd, int inWindowSize,
                            BinaryOutputClass &inControlledRelay);
};

#endif
