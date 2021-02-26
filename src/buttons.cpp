#include "buttons.h"

extern bool radioIsOn;
extern EasyButton btnSnooze;

extern void invokeMainMenu();
extern void setButtonDefaultOff();
extern void setButtonDefaultOn();
extern void selectRadioMode();
extern void exitMainMenu();
extern void paintTopbar();
extern void clearScreen();
extern void formatSPIFFS();
extern void paintTimeModeScreen_reset();
extern void paintRadioScreen_reset();

/*******************************************************/
// Purpose   : Turn on the radio
//           : Wake up ESP32, MP3 decoder, amplifier, etc
// Paramters : None
// Returns   : None
/*******************************************************/
void btnStandbyTurnOn() {
  setEvent(turnOn, now(), LOCAL_TIME);
}

//tmp function
void callFormatSPIFFS() {
  setEvent(formatSPIFFS, now());
}

void turnOn() {
  deleteEvents(); // stop screen updating as soon as possible
  radioIsOn = true;
  resetTFTlight();      // light up TFT
  setButtonDefaultOn(); // change button behaviour

  Serial.println("turnOn():: Need to set to radio mode here");
  Serial.println("turnOn():: Radio ON\n");

  // wake up ESP 32 and power up MP3 decoder and amplifier
  clearScreen();
  paintRadioScreen();

  selectRadioMode();
}

void btnStandbyTurnOff() {
  setEvent(turnOff, now(), LOCAL_TIME);
}

void turnOff() {
  deleteEvents();
  radioIsOn = false;
  // lowerTftLED();
  setButtonDefaultOff();

  Serial.println("turnOff():: Switch to Standby Mode");

  clearScreen();
  paintTimeModeScreen();

  // Make sure that MP3 decoder and amplifier has their power cut out

  // Serial.println("btnStandbyTurnOff():: Going to sleep now"); delay(1000);
//  esp_light_sleep_start();  // Go to sleep now
}

/*******************************************************/
// Purpose   :
// Paramters :
// Returns   :
/*******************************************************/
void btnMenuPressed() {
  Serial.println("btnMenuPressed");
  setEvent(invokeMainMenu, now(), LOCAL_TIME);
}

void btnExitMainMenu() {
  setEvent(exitMainMenu, now(), LOCAL_TIME);
}
/*******************************************************/
// Purpose   : Handles routines when Mode button is pressed.
// Paramters : None
// Returns   : None
/*******************************************************/
void btnModePressed() {
  resetTFTlight();
  setEvent(selectRadioMode, now(), LOCAL_TIME);
}

/*******************************************************/
// Purpose   :
//
// Paramters :
// Returns   :
/*******************************************************/
void btnWeatherPressed() {
  deleteEvents();
  Serial.println("btnWeatherPressed():: Displaying weather screen for " + String(weatherScreenTimeout / 60) + " minute/s");
  resetTFTlight();
  clearScreen();
  btnSnooze.onPressed(btnWeatherExit);

  if (radioIsOn) {
    setEvent(paintRadioScreen_reset, now() + weatherScreenTimeout);     // this needs to be every half a second to update time on screen
  } else {
    setEvent(paintTimeModeScreen_reset, now() + weatherScreenTimeout);  // return to time screen after this time period
  }

  setEvent(paintWeatherModeScreen, now(), LOCAL_TIME);
}

/*******************************************************/
// Purpose   :
//
// Paramters :
// Returns   :
/*******************************************************/
void btnWeatherExit() {
  deleteEvents();
  Serial.println("btnWeatherExit():: Exit Weather screen\n");
  resetTFTlight();
  btnSnooze.onPressed(btnWeatherPressed);

  if (radioIsOn) {
    setEvent(paintRadioScreen_reset, now() );
  } else {
    setEvent(paintTimeModeScreen_reset, now() );
  }
}
