#ifndef MAIN_PAGE_CLASS_h
#define MAIN_PAGE_CLASS_h

#include "Arduino.h"
#include ".//RotaryInputClass.h"
#include ".//BeerReciepeClass.h"
#include ".//BinaryInputClass.h"
#include ".//BrewStandClass.h"
#include <U8glib.h>
#include "bitmaps.h"
#include "./constants.h"

class MainPageClass{
  private:
    int currentCursorLine = 0;
    bool recalcVol = true;;
    char strikeVolLine[20];
    char spargeVolLine[20];

    BrewStandClass *thisBrewStand;
    BeerReciepeClass *thisReciepe;

    void indexLine(int dir);

  public:
    MainPageClass(BrewStandClass &inThisBrewStand,
                  BeerReciepeClass &inThisReciepe);

    void printOn(U8GLIB_ST7920_128X64_1X &lcd, int counter);
    int getUserInput(int &userInput);
};

#endif
