#include "MainPageClass.h"

void MainPageClass::indexLine(int dir){
  if(dir == UP) currentCursorLine--;
  else currentCursorLine++;
  if(currentCursorLine < FIRST_LINE)
    currentCursorLine = LAST_LINE;
  else if(currentCursorLine > LAST_LINE)
    currentCursorLine = FIRST_LINE;
}

int MainPageClass::getUserInput(int &userInput){
  int returnPage = MAIN_PAGE;
  int input = userInput;
  int brewingStage = thisBrewStand->getBrewingStage();

  if(input == LONG_PRESS){
    thisBrewStand->cancelAll();
  }
  else if(input == ROTATE_RIGHT && brewingStage == BOIL)
      thisBrewStand->adjustBoilIntensity(UP);
  else if(input == ROTATE_LEFT && brewingStage == BOIL)
      thisBrewStand->adjustBoilIntensity(DOWN);
  else if(input == ROTATE_RIGHT) indexLine(DOWN);
  else if(input == ROTATE_LEFT)  indexLine(UP);


  else if(input == SHORT_PRESS){
    if(brewingStage == NOT_BREWING){
      if(currentCursorLine == FIRST_LINE){
        recalcVol = true;
        returnPage = RECIEPE_PAGE;
      }
      else if(currentCursorLine == SECOND_LINE){
        thisBrewStand->startBrewing(thisReciepe);
      }
      else if(currentCursorLine == THIRD_LINE){
        thisBrewStand->enableManualControl();
        returnPage = MANUAL_PAGE;
      }
      else if(currentCursorLine == LAST_LINE){
        returnPage = PID_SETTINGS_PAGE;
      }
    }
    else{
      if(currentCursorLine == LAST_LINE){
        thisBrewStand->nextStep();
        thisBrewStand->printTimers();
      }
    }
  }
  return returnPage;
}

void MainPageClass::printOn(U8GLIB_ST7920_128X64_1X &lcd, int counter){
  if(recalcVol){
    recalcVol = false;
    thisReciepe->recalc();
    double vol = thisReciepe->getVolStrikeWater();
    sprintf(strikeVolLine, "%i.%02i gal of water", int(vol),int((vol-floor(vol))*100));
    vol = thisReciepe->getVolSpargeWater();
    sprintf(spargeVolLine, "%i.%02i gal of water", int(vol),int((vol-floor(vol))*100));
  }

  lcd.firstPage();
  do {
    char buf[30];
    lcd.drawBitmapP( -1, 7*currentCursorLine+7, 1, 8, arrow_bitmap);

    lcd.drawStr(0, 6, F("Beer, Engineered"));
    sprintf(buf, "%i", counter);
    lcd.drawStr(105, 6, buf);
    ////////////////////////////////////////////////////////////////////////////
    int brewingStage = thisBrewStand->getBrewingStage();
    if(brewingStage == NOT_BREWING){
      lcd.drawStr(10, 14, F("Change Reciepe"));
      lcd.drawStr(10, 21, F("Start Brewing"));
      lcd.drawStr(10, 28, F("Manual Control"));
      lcd.drawStr(10, 35, F("Change Settings"));
    }
    else if(brewingStage == COLLECTING_STRIKE){
      lcd.drawStr(10, 14, F("Collect Strike Water"));
      lcd.drawStr(10, 21, strikeVolLine);
    }
    else if(brewingStage == HEATING_STRIKE){
      lcd.drawStr(10, 14, F("Heating Strike Water to "));
      sprintf(buf, "%iF", int(thisReciepe->getStrikeTemp()));
      lcd.drawStr(10, 21, buf);
    }
    else if(brewingStage == TRANSFER_STRIKE){
      lcd.drawStr(10, 14, F("Transfer Strike Water"));
    }
    else if(brewingStage == DOUGH_IN){
      lcd.drawStr(10, 14, F("Add Grains to Strike Water"));
    }
    else if(brewingStage == MASH_WHILE_COLLECTING_SPARGE){
      lcd.drawStr(10, 14, F("Mashing Grains"));
      lcd.drawStr(10, 21, F("Collect Sparge Water"));
      lcd.drawStr(10, 28, spargeVolLine);
    }
    else if(brewingStage == MASH_WHILE_HEATING_SPARGE){
      lcd.drawStr(10, 14, F("Mashing Grains"));
      lcd.drawStr(10, 21, F("Heating Sparge Water"));
    }
    else if(brewingStage == MASH_GRAINS){
      lcd.drawStr(10, 14, F("Mashing Grains"));
    }
    else if(brewingStage == MASHOUT_GRAINS){
      lcd.drawStr(10, 14, F("Raising temp for mashout"));
    }
    else if(brewingStage == TRANSFER_FIRST_RUNNING){
      lcd.drawStr(10, 14, F("Transfer First Running"));
    }
    else if(brewingStage == TRANSFER_SPARGE){
      lcd.drawStr(10, 14, F("Transfer Sparge Water"));
    }
    else if(brewingStage == SPARGE_GRAINS){
      lcd.drawStr(10, 14, F("Sparge Grains"));
    }
    else if(brewingStage == TRANSFER_SECOND_RUNNINGS){
      lcd.drawStr(10, 14, F("Transfer Second Running"));
    }
    else if(brewingStage == STARTING_BOIL){
      lcd.drawStr(10, 14, F("Heating Brew Kettle To Boil"));
      lcd.drawStr(10, 21, F("Timer will start soon"));
    }
    else if(brewingStage == BOIL){
      lcd.drawStr(10, 14, F("Watch Water Boil"));
      lcd.drawStr(10, 21, F("Add Hops As Needed"));
      lcd.drawStr(10, 28, F("Drink a Beer"));
      lcd.drawStr(10, 35, F("Rotate Dial for DC"));
    }
    else if(brewingStage == CHILL){
      lcd.drawStr(10, 14, F("Chill Wort"));
    }
    else if(brewingStage == TRANSFER_TO_FERMENTER){
      lcd.drawStr(10, 14, F("Transfer Wort to Fermenter"));
    }
    else if(brewingStage == CLEANING_BREWSTAND){
      lcd.drawStr(10, 14, F("Cleaning Brewstand"));
    }

    ////////////////////////////////////////////////////////////////////////////

    //vertical line
    lcd.drawLine(70, 39, 70, 64);
    lcd.drawStr(72, 46, F("Next Trigger"));

    if( brewingStage == TRANSFER_STRIKE ||
        brewingStage == DOUGH_IN ||
        brewingStage == TRANSFER_FIRST_RUNNING ||
        brewingStage == TRANSFER_SPARGE ||
        brewingStage == TRANSFER_SECOND_RUNNINGS ||
        brewingStage == TRANSFER_TO_FERMENTER ){
      lcd.drawStr(75, 53, F("User Input"));
      lcd.drawStr(10, 35, F("Continue?"));
    }
    else if(brewingStage == COLLECTING_STRIKE ||
            brewingStage == MASH_WHILE_COLLECTING_SPARGE){
      lcd.drawStr(75, 53, F("HLT Volume"));
      lcd.drawStr(10, 35, F("Continue?"));
    }
    else if(brewingStage == HEATING_STRIKE ||
            brewingStage == MASH_WHILE_HEATING_SPARGE){
      lcd.drawStr(75, 53, F("HLT Temp"));
      lcd.drawStr(10, 35, F("Continue?"));
    }
    else if(brewingStage == MASH_GRAINS ||
            brewingStage == MASHOUT_GRAINS ||
            brewingStage == SPARGE_GRAINS ||
            brewingStage == BOIL ){
      lcd.drawStr(75, 53, F("Timer"));
      int totalSec = thisBrewStand->getNextTimer();
      int sec = totalSec % 60;
      int min = totalSec / 60;
      sprintf(buf, "%i:%02i", min, sec);
      lcd.drawStr(75, 60, buf);
    }
    else if(brewingStage == CHILL ||
            brewingStage == STARTING_BOIL){
      lcd.drawStr(75, 53, F("BK Temp"));
    }
    else if(brewingStage == CLEANING_BREWSTAND){

    }

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
    if(brewingStage == BOIL ) sprintf(buf, "%i*", thisBrewStand->getBkDutyCycle());
    else sprintf(buf, "%i", thisBrewStand->getBkSetpoint());
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

MainPageClass::MainPageClass(BrewStandClass &inThisBrewStand,
                             BeerReciepeClass &inThisReciepe){
  thisBrewStand = &inThisBrewStand;
  thisReciepe = &inThisReciepe;
}
