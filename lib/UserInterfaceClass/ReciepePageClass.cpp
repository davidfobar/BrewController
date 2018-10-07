#include "ReciepePageClass.h"

void ReciepePageClass::indexLine(int dir){
  if(dir == UP) currentCursorLine--;
  else currentCursorLine++;

  if(currentCursorLine < GRAIN_BILL_LINE) currentCursorLine = HOP_3_LINE;
  else if(currentCursorLine == HOP_3_LINE+1) currentCursorLine = GRAIN_BILL_LINE;
}

int ReciepePageClass::getUserInput(int &userInput){
  int returnPage = RECIEPE_PAGE;
  int input = userInput;

  if(input == ROTATE_RIGHT && !lineSelected)     indexLine(DOWN);
  else if(input == ROTATE_LEFT && !lineSelected) indexLine(UP);
  else if((input == ROTATE_LEFT || input == ROTATE_RIGHT) && lineSelected){
    int dir;
    if(input == ROTATE_LEFT) dir = DOWN;
    else dir = UP;

    thisReciepe->makeAdjustment(currentCursorLine, dir);
  }
  else if(input == SHORT_PRESS){
    if(currentCursorLine == RETURN_TO_MAIN_LINE) returnPage = MAIN_PAGE;
    else if(!lineSelected) lineSelected = true;
    else if(lineSelected) lineSelected = false;
  }

  return returnPage;
}

void ReciepePageClass::printOn(U8GLIB_ST7920_128X64_1X &lcd){
  lcd.firstPage();
  do {
    char buf[30];
    if(currentCursorLine <= RETURN_TO_MAIN_LINE){
      lcd.drawBitmapP( -1, 8*currentCursorLine+7, 1, 8, arrow_bitmap);
    }
    else{
      lcd.drawBitmapP( 105, 8*currentCursorLine-25, 1, 8, arrow_bitmap);
    }

    lcd.drawStr(0,6,F("Beer, Engineered"));
    lcd.drawLine(80,0,80,64);

    if(currentCursorLine == GRAIN_BILL_LINE && lineSelected) lcd.drawLine(82, 15, 102, 15);
    lcd.drawStr(10,14, F("Grain Bill Wieght"));
    double grainWeight = thisReciepe->getGrainWeight();
    sprintf(buf, "%i.%i", int(grainWeight), int((grainWeight - floor(grainWeight))*10));
    lcd.drawStr(82, 14, buf);

    if(currentCursorLine == MASH_TEMP_LINE && lineSelected) lcd.drawLine(82, 23, 102, 23);
    lcd.drawStr(10,22, F("Mash Temp"));
    double mashTemp = thisReciepe->getMashTemp();
    sprintf(buf, "%i", int(mashTemp));
    lcd.drawStr(82, 22, buf);

    if(currentCursorLine == MASH_LENGTH_LINE && lineSelected) lcd.drawLine(82, 31, 102, 31);
    lcd.drawStr(10,30, F("Mash Length"));
    int mashLength = thisReciepe->getMashLenght();
    sprintf(buf, "%i", int(mashLength));
    lcd.drawStr(82, 30, buf);

    if(currentCursorLine == MASH_OUT_TEMP_LINE && lineSelected) lcd.drawLine(82, 39, 102, 39);
    lcd.drawStr(10,38, F("Mash Out Temp"));
    double mashoutTemp = thisReciepe->getMashoutTemp();
    sprintf(buf, "%i", int(mashoutTemp));
    lcd.drawStr(82, 38, buf);

    if(currentCursorLine == BOIL_SCHEDULE_LINE && lineSelected) lcd.drawLine(82, 47, 102, 47);
    lcd.drawStr(10,46, F("Boil Length"));
    int boilLength = thisReciepe->getBoilLength();
    sprintf(buf, "%i", int(boilLength));
    lcd.drawStr(82, 46, buf);

    if(currentCursorLine == CHILL_TEMP_LINE && lineSelected) lcd.drawLine(82, 55, 102, 55);
    lcd.drawStr(10,54, F("Chill Temp"));
    double chillTemp = thisReciepe->getChillingTemp();
    sprintf(buf, "%i", int(chillTemp));
    lcd.drawStr(82, 54, buf);

    lcd.drawStr(109, 28, F("Hops"));
    int *hopAddition = thisReciepe->getHopSchedule();
    lcd.drawLine(109, 29, 128, 29);
    sprintf(buf, "%i", boilLength - int(hopAddition[0]));
    lcd.drawStr(115, 38, buf);
    if(currentCursorLine == HOP_0_LINE && lineSelected) lcd.drawLine(115, 39, 124, 39);

    sprintf(buf, "%i", boilLength - int(hopAddition[1]));
    lcd.drawStr(115, 46, buf);
    if(currentCursorLine == HOP_1_LINE && lineSelected) lcd.drawLine(115, 47, 124, 47);

    sprintf(buf, "%i", boilLength - int(hopAddition[2]));
    lcd.drawStr(115, 54, buf);
    if(currentCursorLine == HOP_2_LINE && lineSelected) lcd.drawLine(115, 55, 124, 55);

    sprintf(buf, "%i", boilLength - int(hopAddition[3]));
    lcd.drawStr(115, 62, buf);
    if(currentCursorLine == HOP_3_LINE && lineSelected) lcd.drawLine(115, 63, 124, 63);

    lcd.drawStr(10,62, F("Return to Main"));
  } while( lcd.nextPage() );
}
