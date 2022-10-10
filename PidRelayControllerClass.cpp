#include "./PidRelayControllerClass.h"

void PidRelayControllerClass::changeSetpointTo(double newSetpoint){
  setpoint = newSetpoint;
}

void PidRelayControllerClass::changeTunings(double inKp, double inKi, double inKd){
  Kp = inKp;
  Ki = inKi;
  Kd = inKd;

  _PID.SetTunings(Kp, Ki, Kd);
}

void PidRelayControllerClass::changeWindowSize(int inWindowSize){
  windowSize = inWindowSize;
  _PID.SetOutputLimits(0, windowSize);
}

void PidRelayControllerClass::standby(){
  controlledRelay->turnOff();
}

void PidRelayControllerClass::checkPIDandOperateRelay(){
  unsigned long currentTime = millis();

  if(PIDisActive){
    input = inputSource->getTemperature();
    _PID.Compute();

    /************************************************
     * turn the relay on/off based on pid output
     ************************************************/
    if(currentTime - windowStartTime > windowSize){
      //time to shift the Relay Window
      windowStartTime += windowSize;
    }
    if(outputLength > currentTime - windowStartTime){
      controlledRelay->turnOn();
    }
    else {
      controlledRelay->turnOff();
    }
  }
  else if(manual){
    if(currentTime - windowStartTime > windowSize){
      //time to shift the Relay Window
      windowStartTime += windowSize;
    }
    if(dutyCycle/100.0*windowSize > currentTime - windowStartTime){
      controlledRelay->turnOn();
    }
    else controlledRelay->turnOff();
  }
  else controlledRelay->turnOff();
}

void PidRelayControllerClass::enableWithSetpoint(double newSetpoint){
  PIDisActive = true;
  manual = false;
  setpoint = newSetpoint;
  windowStartTime = millis();
}

void PidRelayControllerClass::disable(){
  PIDisActive = false;
  manual = false;
  setpoint = 0;
  controlledRelay->turnOff();
}

bool PidRelayControllerClass::isActive(){
  return PIDisActive;
}

void PidRelayControllerClass::enableManual(){
  manual = true;
  dutyCycle = 0;
  windowStartTime = millis();
}

void PidRelayControllerClass::adjustDC(int dir){
  if(dir == UP){
    if(dutyCycle < 100) dutyCycle += 5;
  }
  else{
    if(dutyCycle > 0) dutyCycle--;
  }
}

void PidRelayControllerClass::setDCtoZero(){
  dutyCycle = 0;
}

int PidRelayControllerClass::getDutyCycle(){
  if(manual) return dutyCycle;
  else return outputLength;
}

PidRelayControllerClass::PidRelayControllerClass(TemperatureClass &source,
                        double inKp, double inKi,
                        double inKd, int inWindowSize,
                        BinaryOutputClass &inControlledRelay) :
  inputSource(&source), Kp(inKp), Ki(inKi), Kd(inKd),
  windowSize(inWindowSize), PIDisActive(false),
  controlledRelay(&inControlledRelay),
  _PID(&input, &outputLength, &setpoint, Kp, Ki, Kd, DIRECT){
    _PID.SetOutputLimits(0, windowSize);
    _PID.SetMode(AUTOMATIC);
    controlledRelay->turnOff();
    dutyCycle = 0;
  }
