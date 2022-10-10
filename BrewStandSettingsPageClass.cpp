#include "BrewStandSettingsPageClass.h"

BrewStandSettingsPageClass::BrewStandSettingsPageClass(BrewStandClass &inThisBrewStand){
  thisBrewStand = &inThisBrewStand;
}

void BrewStandSettingsPageClass::indexLine(int dir){
  if(dir == UP) currentCursorLine--;
  else currentCursorLine++;
  if(currentCursorLine < 0) currentCursorLine = RETURN_TO_MAIN_LINE;
  else if(currentCursorLine > RETURN_TO_MAIN_LINE) currentCursorLine = 0;
}

int BrewStandSettingsPageClass::getUserInput(int &userInput){
  int returnPage = PID_SETTINGS_PAGE;
  int input = userInput;

  if(input == ROTATE_RIGHT && !lineSelected)     indexLine(DOWN);
  else if(input == ROTATE_LEFT && !lineSelected) indexLine(UP);
  else if((input == ROTATE_LEFT || input == ROTATE_RIGHT) && lineSelected){
    int dir;
    if(input == ROTATE_LEFT) dir = DOWN;
    else dir = UP;

    thisBrewStand->changeSetting(currentCursorLine, dir);
  }
  else if(input == SHORT_PRESS){
    if(currentCursorLine == RETURN_TO_MAIN_LINE){
      returnPage = MAIN_PAGE;
      thisBrewStand->saveBrewStandSettings();
    }
    else if(!lineSelected) lineSelected = true;
    else if(lineSelected) lineSelected = false;
  }
  else if(input == LONG_PRESS){
    returnPage = MAIN_PAGE;
    thisBrewStand->saveBrewStandSettings();
  }

  return returnPage;
}

void BrewStandSettingsPageClass::printOn(U8GLIB_ST7920_128X64_1X &lcd){
  lcd.firstPage();
  char buf[30];
  double temp;
  do {
    if(currentCursorLine <= BOIL_OFF_RATE){
      lcd.drawBitmapP( -1, 8*currentCursorLine+15, 1, 8, arrow_bitmap);
    }
    else if(currentCursorLine <= SPARGE_TRANS_TEMP_LOSE){
      lcd.drawBitmapP( 62, 8*currentCursorLine-25, 1, 8, arrow_bitmap);
    }
    else if(currentCursorLine == RETURN_TO_MAIN_LINE){
      lcd.drawBitmapP( -1, 56, 1, 8, arrow_bitmap);
    }
    else {
      if(currentCursorLine == HLT_P || currentCursorLine == RIMS_P ||
         currentCursorLine == BK_P){
          lcd.drawLine(85, 40, 95, 40);
      }
      else if(currentCursorLine == HLT_I || currentCursorLine == RIMS_I ||
         currentCursorLine == BK_I){
          lcd.drawLine(107, 40, 114, 40);
      }
      else if(currentCursorLine == HLT_D || currentCursorLine == RIMS_D ||
         currentCursorLine == BK_D){
          lcd.drawLine(119, 40, 128, 40);
      }

      if(currentCursorLine == HLT_P || currentCursorLine == HLT_I ||
         currentCursorLine == HLT_D){
           lcd.drawLine(64, 63, 75, 63);
      }
      else if(currentCursorLine == RIMS_P || currentCursorLine == RIMS_I ||
         currentCursorLine == RIMS_D){
           lcd.drawLine(64, 56, 83, 56);
      }
      else if(currentCursorLine == BK_P || currentCursorLine == BK_I ||
         currentCursorLine == BK_D){
           lcd.drawLine(64, 48, 75, 48);
      }
    }

    lcd.drawStr(0,6,F("Change Brewery Settings"));

    lcd.drawStr(0, 14, F("Vol loss"));
    lcd.drawStr(8, 22, F("Gr. abs"));
    temp = thisBrewStand->getSetting(GRAIN_ABSORBPTION);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(45, 22, buf);

    lcd.drawStr(8, 30, F("BK D.S."));
    temp = thisBrewStand->getSetting(KETTLE_DEAD_SPACE);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(45, 30, buf);

    lcd.drawStr(8, 38, F("HLT->MLT"));
    temp = thisBrewStand->getSetting(HLT_MLT_TRANSFER_LOSE);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(45, 38, buf);

    lcd.drawStr(8, 46, F("MLT->BK"));
    temp = thisBrewStand->getSetting(MLT_BK_TRANSFER_LOSE);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(45, 46, buf);

    lcd.drawStr(8, 54, F("Boil off"));
    temp = thisBrewStand->getSetting(BOIL_OFF_RATE);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(45, 54, buf);

    lcd.drawLine(0, 55, 62, 55);
    lcd.drawStr(8, 63, F("Done"));
    lcd.drawLine(62, 8, 62, 64);

    lcd.drawStr(64, 14, F("Temp loss"));
    lcd.drawStr(72, 22, F("Gr. abs"));
    temp = thisBrewStand->getSetting(GRAIN_TEMP_ABS_FACTOR);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(110, 22, buf);

    lcd.drawStr(72, 30, F("HLT->MLT"));
    temp = thisBrewStand->getSetting(SPARGE_TRANS_TEMP_LOSE);
    sprintf(buf, "%i.%02i", int(temp), int((temp - floor(temp))*100));
    lcd.drawStr(110, 30, buf);

    lcd.drawLine(62, 32, 128, 32);

    lcd.drawStr(64, 39, F("PID:"));
    lcd.drawStr(85, 39, F("Kp"));
    lcd.drawStr(107, 39, F("Ki"));
    lcd.drawStr(119, 39, F("Kd"));
    lcd.drawStr(64, 47, F("BK"));
    lcd.drawStr(64, 55, F("RIMS"));
    lcd.drawStr(64, 63, F("HLT"));

    temp = thisBrewStand->getSetting(BK_P);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(85, 47, buf);
    if(lineSelected && currentCursorLine == BK_P) lcd.drawLine(85, 48, 95, 48);

    temp = thisBrewStand->getSetting(RIMS_P);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(85, 55, buf);
    if(lineSelected && currentCursorLine == RIMS_P) lcd.drawLine(85, 56, 95, 56);

    temp = thisBrewStand->getSetting(HLT_P);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(85, 63, buf);
    if(lineSelected && currentCursorLine == HLT_P) lcd.drawLine(85, 63, 95, 63);

    temp = thisBrewStand->getSetting(BK_I);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(107, 47, buf);
    if(lineSelected && currentCursorLine == BK_I) lcd.drawLine(107, 48, 114, 48);

    temp = thisBrewStand->getSetting(RIMS_I);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(107, 55, buf);
    if(lineSelected && currentCursorLine == RIMS_I) lcd.drawLine(107, 56, 114, 56);

    temp = thisBrewStand->getSetting(HLT_I);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(107, 63, buf);
    if(lineSelected && currentCursorLine == HLT_I) lcd.drawLine(107, 63, 114, 63);

    temp = thisBrewStand->getSetting(BK_D);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(119, 47, buf);
    if(lineSelected && currentCursorLine == BK_D) lcd.drawLine(119, 48, 128, 48);

    temp = thisBrewStand->getSetting(RIMS_D);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(119, 55, buf);
    if(lineSelected && currentCursorLine == RIMS_D) lcd.drawLine(119, 56, 128, 56);

    temp = thisBrewStand->getSetting(HLT_D);
    sprintf(buf, "%i", int(temp));
    lcd.drawStr(119, 63, buf);
    if(lineSelected && currentCursorLine == HLT_D) lcd.drawLine(119, 63, 128, 63);

  } while( lcd.nextPage() );
}
