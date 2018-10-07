#ifndef BINARY_INPUT_CLASS_h
#define BINARY_INPUT_CLASS_h

#include "Arduino.h"
#include "../constants.h"

const unsigned int DEBOUNCE_TIME = 10; //milliseconds

class BinaryInputClass{
  private:
    bool state;
    int pin;
    unsigned long changeTime;
    bool debouncing;

  public:
    BinaryInputClass(int inPin);

    void updateStatus();

    bool getState();
};

#endif
