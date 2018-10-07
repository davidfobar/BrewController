#ifndef BITMAPS_inl
#define BITMAPS_inl

#include "Arduino.h"

const uint8_t lock_bitmap[] U8G_PROGMEM = {
  0b0011100,
  0b0100010,
  0b0100010,
  0b1111111,
  0b1110111,
  0b1100011,
  0b1110111,
  0b1111111
};
const uint8_t arrow_bitmap[] U8G_PROGMEM = {
  0b0000000,
  0b0001000,
  0b0001100,
  0b0001110,
  0b1111111,
  0b0001110,
  0b0001100,
  0b0001000
};
const uint8_t select_bitmap[] U8G_PROGMEM = {
  0b0000000,
  0b0011100,
  0b0111110,
  0b1111111,
  0b1111111,
  0b1111111,
  0b0111110,
  0b0011100
};

const uint8_t upArrow_bitmap[] U8G_PROGMEM = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100
};

const uint8_t downArrow_bitmap[] U8G_PROGMEM = {
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};

#endif
