#ifndef MANUAL_PAGE_CLASS_h
#define MANUAL_PAGE_CLASS_h

#include "Arduino.h"
#include "./RotaryInputClass.h"
#include "./BrewStandClass.h"
#include <U8glib.h>
#include "bitmaps.h"
#include "./constants.h"

const int MANUAL_CONTROL_NONE = 0;
const int MANUAL_CONTROL_BK = 1;
const int MANUAL_CONTROL_RIMS = 2;
const int MANUAL_CONTROL_HLT = 3;

class ManualPageClass{
  private:
    int currentCursorLine = 0;
    int manualControlChoice = MANUAL_CONTROL_NONE;

    BrewStandClass *thisBrewStand;

    void indexLine(int dir);

  public:
    ManualPageClass(BrewStandClass &inThisBrewStand);

    void printOn(U8GLIB_ST7920_128X64_1X &lcd, int counter);
    int getUserInput(int &userInput);
};

#endif
