#include "encoder_routines.h"

extern ESPRotary selector;
extern ESPRotary volume;

extern void menuSelect();

/*******************************************************/
/**************   VOLUME   *****************************/
/*******************************************************/
// Purpose   : Mutes / Unmutes audio volume
// Paramters : None
// Returns   : None
/*******************************************************/
void IRAM_ATTR btnVolumePressed() {
  static int vol_last_position;
  static int vol_this_position;
  vol_this_position = volume.getPosition();
  if (vol_this_position == 0) {
    volume.resetPosition(vol_last_position);
  } else {
    vol_last_position = vol_this_position;
    volume.resetPosition(); // set to 0 (Mute)
  }
  resetTFTlight();
  Serial.printf("Volume: (%u) \n", volume.getPosition());
}

/*******************************************************/
// Purpose   :
// Paramters :
// Returns   :
/*******************************************************/
void IRAM_ATTR volumeChanged(ESPRotary& r) {
  resetTFTlight();
  Serial.print("Volume   : ");
  Serial.println(r.getPosition());
}

/*******************************************************/
/************   SELECTOR   *****************************/
/*******************************************************/
// Purpose   : Controls channel selection
// Paramters : None
// Returns   : None
/*******************************************************/
void IRAM_ATTR btnSelectorPressed() {
  resetTFTlight();
}

void btnMenuSelect() { // selector encoder button pressed when in menu
  setEvent(menuSelect, now(), LOCAL_TIME);
}

/*******************************************************/
// Purpose   : Controls channel selection
// Paramters : None
// Returns   : None
/*******************************************************/
void IRAM_ATTR selectorChanged(ESPRotary& r) {
  resetTFTlight();
}
