#ifndef BREW_STAND_SETTINGS_PAGE_h
#define BREW_STAND_SETTINGS_PAGE_h

#include "Arduino.h"
#include "../RotaryInputClass/RotaryInputClass.h"
#include "../BrewStandClass/BrewStandClass.h"
#include <U8glib.h>
#include "bitmaps.inl"
#include "../constants.h"

class BrewStandSettingsPageClass{
  private:
    int currentCursorLine = 0;
    bool lineSelected = false;
    const int RETURN_TO_MAIN_LINE = 16;
    BrewStandClass *thisBrewStand;

  public:
    BrewStandSettingsPageClass(BrewStandClass &inThisBrewStand);
    void indexLine(int dir);
    void printOn(U8GLIB_ST7920_128X64_1X &lcd);
    int getUserInput(int &userInput);
};

#endif
