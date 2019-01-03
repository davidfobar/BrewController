#ifndef BINARY_OUTPUT_CLASS_h
#define BINARY_OUTPUT_CLASS_h

#include "Arduino.h"
#include "../constants.h"
//#include "../BinaryInputClass/BinaryInputClass.h"

const int ACTIVE_HIGH = 1;

class BinaryOutputClass{
  private:
    int outputPin;
    int state;
    int logicType;

  public:
    //sets the arduino pin using pinMode(), if inLogicType == 1, then the OUTPUT
    //  pin is held high when turnOn is executed.
    BinaryOutputClass(int outPin, int inLogicType = 1);

    //returns the current state of the output
    bool getState();

    //executes the given command as expected.
    void toggle();
    void turnOn();
    void turnOff();
};
#endif
