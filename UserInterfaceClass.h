#ifndef USER_INTFACE_CLASS_h
#define USER_INTFACE_CLASS_h

#include "Arduino.h"
#include "./BrewStandClass.h"
#include "./RotaryInputClass.h"
#include "./BeerReciepeClass.h"
#include "MainPageClass.h"
#include "ReciepePageClass.h"
#include "BrewStandSettingsPageClass.h"
#include "ManualPageClass.h"
#include <U8glib.h>
#include "./constants.h"

const int MIN_REFRESH_INTERVAL = 200; //ms
const int MAX_REFRESH_INTERVAL = 500; //ms

class UserInterfaceClass{
  private:
    RotaryInputClass rotaryUserInput;

    unsigned long prevRefreshTime;
    bool refreshRequired = true;
    bool alarmActive = false;

    int currentPage = MAIN_PAGE;
    MainPageClass mainPage;
    ReciepePageClass reciepePage;
    BrewStandSettingsPageClass brewStandSettingsPage;
    ManualPageClass manualPage;
    U8GLIB_ST7920_128X64_1X lcd;
    BrewStandClass *thisBrewStand;

    void getUserInput();

  public:
    UserInterfaceClass(BrewStandClass &inThisBrewStand,
                       BeerReciepeClass &inThisReciepe);

    void update();

    void refresh(int &counter);
};

#endif
