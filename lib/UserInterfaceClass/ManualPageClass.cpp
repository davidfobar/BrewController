#include "ManualPageClass.h"

void ManualPageClass::indexLine(int dir){
  if(dir == DOWN) currentCursorLine--;
  else currentCursorLine++;
  if(currentCursorLine < FIRST_LINE)
    currentCursorLine = LAST_LINE;
  else if(currentCursorLine > LAST_LINE)
    currentCursorLine = FIRST_LINE;
}

int ManualPageClass::getUserInput(int &userInput){
  int returnPage = MANUAL_PAGE;
  int input = userInput;

  if(input == LONG_PRESS){
    if(manualControlChoice == MANUAL_CONTROL_NONE){
      thisBrewStand->stopManualControl();
      thisBrewStand->cancelAll();
      returnPage = MAIN_PAGE;
    }
    else if(manualControlChoice == MANUAL_CONTROL_BK){
      thisBrewStand->stopBoilCoilPID();
    }
    else if(manualControlChoice == MANUAL_CONTROL_RIMS){
      thisBrewStand->stopRimsPID();
    }
    else if(manualControlChoice == MANUAL_CONTROL_HLT){
      thisBrewStand->stopHltPID();
    }
    manualControlChoice = MANUAL_CONTROL_NONE;
  }

  else if(input == ROTATE_LEFT || input == ROTATE_RIGHT){
    int dir;
    if(input == ROTATE_LEFT) dir = DOWN;
    else dir = UP;

    if(manualControlChoice == MANUAL_CONTROL_BK){
      thisBrewStand->stopHltPID();
      thisBrewStand->adjustBoilIntensity(dir);
    }
    else if(manualControlChoice == MANUAL_CONTROL_RIMS){
      thisBrewStand->changeSetting(RIMS_SETPOINT, dir);
    }
    else if(manualControlChoice == MANUAL_CONTROL_HLT){
      thisBrewStand->changeSetting(HLT_SETPOINT, dir);
      thisBrewStand->stopBoilCoilPID();
    }
    else indexLine(dir);
  }
  else if(input == SHORT_PRESS){
    if(manualControlChoice == MANUAL_CONTROL_NONE){
      if(currentCursorLine == SECOND_LINE) manualControlChoice = MANUAL_CONTROL_BK;
      if(currentCursorLine == THIRD_LINE) manualControlChoice = MANUAL_CONTROL_RIMS;
      if(currentCursorLine == LAST_LINE) manualControlChoice = MANUAL_CONTROL_HLT;
    }
    else manualControlChoice = MANUAL_CONTROL_NONE;
  }
  return returnPage;
}

void ManualPageClass::printOn(U8GLIB_ST7920_128X64_1X &lcd, int counter){
  lcd.firstPage();
  do {
    char buf[30];
    lcd.drawBitmapP( -1, 7*currentCursorLine+7, 1, 8, arrow_bitmap);

    lcd.drawStr(0, 6, F("Beer, Engineered"));
    sprintf(buf, "%i", counter);
    lcd.drawStr(105, 6, buf);
    ////////////////////////////////////////////////////////////////////////////

    lcd.drawStr(10, 14, F("Manual control"));
    if(manualControlChoice == MANUAL_CONTROL_BK){
      lcd.drawStr(10, 21, F("Change BK duty cycle now"));
    }
    else lcd.drawStr(10, 21, F("Control BK"));

    if(manualControlChoice == MANUAL_CONTROL_RIMS){
      lcd.drawStr(10, 28, F("Change RIMS temp now"));
    }
    else lcd.drawStr(10, 28, F("Control RIMS"));

    if(manualControlChoice == MANUAL_CONTROL_HLT){
      lcd.drawStr(10, 35, F("Change HLT temp now"));
    }
    else lcd.drawStr(10, 35, F("Control HLT"));

    ////////////////////////////////////////////////////////////////////////////

    //vertical line
    lcd.drawLine(70, 39, 70, 64);

    //show required adjustments to rims tube flow
    if(thisBrewStand->rimsActive()){
      int flowRateLevel = thisBrewStand->checkRimsFlow();
      if(flowRateLevel == TOO_SLOW)
        lcd.drawBitmapP(120, 50, 1, 5, upArrow_bitmap);
      if(flowRateLevel == TOO_FAST)
        lcd.drawBitmapP(120, 58, 1, 5, downArrow_bitmap);
    }

    ////////////////////////////////////////////////////////////////////////////

    //horizontal line
    lcd.drawLine(0,39,128,39);

    lcd.drawStr(0, 46, F("VESSEL"));
    lcd.drawStr(0, 52, F("BKT:"));
    lcd.drawStr(0, 58, F("RIM:"));
    lcd.drawStr(0, 64, F("HLT:"));

    lcd.drawStr(30, 46, F("SP"));
    sprintf(buf, "%i*", thisBrewStand->getBkDutyCycle());
    lcd.drawStr(30, 52, buf);
    sprintf(buf, "%i", thisBrewStand->getRimsSetpoint());
    lcd.drawStr(30, 58, buf);
    sprintf(buf, "%i", thisBrewStand->getHltSetpoint());
    lcd.drawStr(30, 64, buf);

    lcd.drawStr(48, 46, F("Temp"));
    sprintf(buf, "%i", thisBrewStand->getBkTemp());
    lcd.drawStr(48, 52, buf);
    sprintf(buf, "%i", thisBrewStand->getRimsTemp());
    lcd.drawStr(48, 58, buf);
    sprintf(buf, "%i", thisBrewStand->getHltTemp());
    lcd.drawStr(48, 64, buf);

  } while(lcd.nextPage());
}

ManualPageClass::ManualPageClass(BrewStandClass &inThisBrewStand){
  thisBrewStand = &inThisBrewStand;
}
