#ifndef CONSTANTS_h
#define CONSTANTS_h

#include "Arduino.h"

const int MILLIS_PER_SECOND = 1000;
const int SEC_PER_MIN = 60;
const int MIN_PER_HOUR = 60;

//stages of brewing
const int NOT_BREWING                  = 0;
const int COLLECTING_STRIKE            = 1;
const int HEATING_STRIKE               = 2;
const int TRANSFER_STRIKE              = 3;
const int DOUGH_IN                     = 4;
const int MASH_WHILE_COLLECTING_SPARGE = 5;
const int MASH_WHILE_HEATING_SPARGE    = 6;
const int MASH_GRAINS                  = 7;
const int MASHOUT_GRAINS               = 8;
const int TRANSFER_FIRST_RUNNING       = 9;
const int TRANSFER_SPARGE              = 10;
const int SPARGE_GRAINS                = 11;
const int TRANSFER_SECOND_RUNNINGS     = 12;
const int STARTING_BOIL                = 13;
const int BOIL                         = 14;
const int SANITIZE_PUMP                = 15;
const int CHILL                        = 16;
const int TRANSFER_TO_FERMENTER        = 17;
const int CLEANING_BREWSTAND           = 18;
const int MANUAL_CONTROL               = 19;

const int ADD_HOP0 = 20;
const int ADD_HOP1 = 21;
const int ADD_HOP2 = 22;
const int ADD_HOP3 = 23;

const int NO_UPDATE = 0;

//commonly used values
const int DOWN = 0;
const int UP = 1;
const int LEFT = 0;
const int RIGHT = 1;
const int OFF = 0;
const int ON  = 1;

//pins used for Arduino Mega

//outputs
const int HLT_HEAT_PIN = 9;
const int WORT_PUMP_PIN = 11;
const int BOIL_COIL_PIN = 8;
const int RIMS_HEATER_PIN = 12;
const int WATER_PUMP_PIN = 10;

//inputs
const int ONE_WIRE_BUS = 3;
const int RIMS_FLOW_INPUT_PIN = 2;
const int WATER_IN_INPUT_PIN = 1;
const int BOIL_COIL_SWITCH_PIN = 31;
const int HLT_HEAT_SWITCH_PIN = 33;
const int WATER_PUMP_SWITCH_PIN = 35;
const int WORT_PUMP_SWITCH_PIN = 37;
const int RIMS_HEAT_SWITCH_PIN = 39;


//user interface
const int LCD_CS = 49;
const int BTN_ENC = 30; //35
const int BTN_EN1 = 24; //33
const int BTN_EN2 = 28; //31

const int ALARM_PIN = 22;
const int SD_SELECT_PIN = 26;
const int SD_DETECT_PIN = 48;

//list of the various pages
const int MAIN_PAGE = 0;
const int RECIEPE_PAGE = 1;
const int PID_SETTINGS_PAGE = 2;
const int MANUAL_PAGE = 3;

//main page line options
const int FIRST_LINE = 0;
const int SECOND_LINE = 1;
const int THIRD_LINE = 2;
const int LAST_LINE = 3;


#endif
