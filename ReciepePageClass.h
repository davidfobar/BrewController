#ifndef RECIEPE_PAGE_CLASS_h
#define RECIEPE_PAGE_CLASS_h

#include "Arduino.h"
#include "./RotaryInputClass.h"
#include "./BeerReciepeClass.h"
#include <U8glib.h>
#include "bitmaps.h"
#include "./constants.h"

class ReciepePageClass{
  private:
    int currentCursorLine = 0;
    const int RETURN_TO_MAIN_LINE = 6;
    bool lineSelected = false;
    BeerReciepeClass *thisReciepe;
    void indexLine(int dir);
    void incrementValue(double &val, int dir);
    void incrementValue(int &val, int dir);

  public:
    ReciepePageClass(BeerReciepeClass &inThisReciepe){
      thisReciepe = &inThisReciepe;
    }

    void printOn(U8GLIB_ST7920_128X64_1X &lcd);
    int getUserInput(int &userInput);
};

#endif
