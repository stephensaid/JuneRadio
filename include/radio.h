#ifndef radio_h
#define radio_h

#include <Arduino.h>

/********* ENUMERATIONS ****************/
// last is not a mode... it only serves as an end to the list
// Will set mode to first in list when the end of the list is reached
enum radioModeEnum {
  internetRadio,
  dabRadio,
  fmRadio,
  mp3,
  savedStations,
  last = savedStations
} radioMode;          // enumaration for modes

void selectRadioMode();

#endif // radio_h
