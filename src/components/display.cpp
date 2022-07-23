#include <Arduino.h>
#include "components/display.h"

Display::Display() {
    Serial1.begin(2400);
}

void Display::update(unsigned long currentMillis) {  
  if (currentMillis - this->lastRTIWrite < RTI_DELAY) return;

  rtiWrite(this->display_modes[this->current_display_mode]);
  
  if (this->send_brightness) rtiWrite(this->brightness_levels[current_brightness_level]);
  else rtiWrite(0x40);
  
  rtiWrite(0x83); // ALWAYS NEEDS TO BE SEND LAST!
  this->lastRTIWrite = currentMillis;
}

// Turns the screen off.
void Display::screenOFF() {
  current_display_mode = RTI_OFF;
}

// Sets the screen to RGB input mode.
void Display::screenRGB() {
  current_display_mode = RTI_RGB;
}

// Sets the screen to PAL input mode.
void Display::screenPAL() {
  current_display_mode = RTI_PAL;
}

// Sets the screen to NTSC input mode.
void Display::screenNTSC() {
  current_display_mode = RTI_NTSC;
}

// Write a byte to the RTI display unit.
void Display::rtiWrite(char byte) {
  Serial1.print(byte);
}