#include "radio.h"

extern void paintRadioScreen();

/*******************************************************/
// Purpose   : Sets radio mode and stars playback
//             1. Iterates through radio modes every time
//             Mode button is pressed. Goes to first option
//             if the last option is reached.
// Paramters : None
// Returns   : None
/*******************************************************/
void selectRadioMode() {
  radioMode = radioModeEnum(radioMode + 1);

  if (radioMode == radioModeEnum(last))
    radioMode = radioModeEnum(0);

  Serial.printf("radio mode: %u \n", radioMode);

  // set varibales
  switch (radioMode) {
    case internetRadio:

      break;
    case dabRadio:

      break;
    case fmRadio:

      break;
    case mp3:

      break;
    case savedStations:

      break;
  }

  // update screen
  paintRadioScreen();

  // set hardware for playing radio mode.
  // ...
}
