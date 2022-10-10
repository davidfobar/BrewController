#include "UserInterfaceClass.h"

void UserInterfaceClass::getUserInput(){
  int input = rotaryUserInput.getUserInput();

  if(currentPage == MAIN_PAGE){
    currentPage = mainPage.getUserInput(input);
  }
  else if(currentPage == RECIEPE_PAGE){
    currentPage = reciepePage.getUserInput(input);
  }
  else if(currentPage == PID_SETTINGS_PAGE){
    currentPage = brewStandSettingsPage.getUserInput(input);
  }
  else if(currentPage == MANUAL_PAGE){
    currentPage = manualPage.getUserInput(input);
  }
  refreshRequired = true;
}

void UserInterfaceClass::update(){
  //check for user rotary input and take action
    if(rotaryUserInput.update()) getUserInput();
}

void UserInterfaceClass::refresh(int &counter){
  if((refreshRequired && (millis() - prevRefreshTime > MIN_REFRESH_INTERVAL)) ||
      millis() - prevRefreshTime > MAX_REFRESH_INTERVAL){
    refreshRequired = false;
    prevRefreshTime = millis();

    if(currentPage == MAIN_PAGE) mainPage.printOn(lcd, counter);
    else if(currentPage == RECIEPE_PAGE) reciepePage.printOn(lcd);
    else if(currentPage == PID_SETTINGS_PAGE) brewStandSettingsPage.printOn(lcd);
    else if(currentPage == MANUAL_PAGE) manualPage.printOn(lcd, counter);
    counter = 0;
  }
}

UserInterfaceClass::UserInterfaceClass(BrewStandClass &inThisBrewStand,
                   BeerReciepeClass &inThisReciepe) :
                      rotaryUserInput(BTN_EN1, BTN_EN2, BTN_ENC),
                      mainPage(inThisBrewStand, inThisReciepe),
                      reciepePage(inThisReciepe),
                      brewStandSettingsPage(inThisBrewStand),
                      manualPage(inThisBrewStand),
                      lcd(LCD_CS){
  lcd.setFont(u8g_font_04b_03);
	lcd.setColorIndex(1); // Instructs the display to draw with a pixel on.
  delay(1);
  rotaryUserInput.enable();
  thisBrewStand = &inThisBrewStand;
}
