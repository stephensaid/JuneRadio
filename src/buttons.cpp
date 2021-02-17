#include "buttons.h"

extern bool isOn;
extern EasyButton btnSnooze;

extern void invokeMainMenu();
extern void setButtonDefaultOff();
extern void setButtonDefaultOn();
extern void selectRadioMode();
extern void exitMainMenu();


/*******************************************************/
// Purpose   : Turn on the radio
//           : Wake up ESP32, MP3 decoder, amplifier, etc
// Paramters : None
// Returns   : None
/*******************************************************/
void btnStandbyTurnOn() {
  nowEvent = setEvent(turnOn, now(), LOCAL_TIME);
}

void turnOn() {
  resetTFTlight();
  isOn = true;
  setButtonDefaultOn();

  Serial.println("btnStandbyTurnOn()::Need to set to radio mode here");
  Serial.println("btnStandbyTurnOn()::Radio ON");

  // wake up ESP 32 and power up MP3 decoder and amplifier
  tft.fillScreen(con.element.BG_COLOUR);
  selectRadioMode();
}

void btnStandbyTurnOff() {
  nowEvent = setEvent(turnOff, now(), LOCAL_TIME);
}

void turnOff() {
  isOn = false;
  setButtonDefaultOff();
  lowerTftLED();
  tft.fillScreen(con.element.BG_COLOUR);
  paintTimeModeScreen();

  // Make sure that MP3 decoder and amplifier has their power cut out

  Serial.println("Going to sleep now"); delay(1000);
//  esp_light_sleep_start();  // Go to sleep now
}

/*******************************************************/
// Purpose   :
// Paramters :
// Returns   :
/*******************************************************/
void btnMenuPressed() {
  Serial.println("btnMenuPressed");
  nowEvent = setEvent(invokeMainMenu, now(), LOCAL_TIME);
}

void btnExitMainMenu() {
  nowEvent = setEvent(exitMainMenu, now(), LOCAL_TIME);
}
/*******************************************************/
// Purpose   : Handles routines when Mode button is pressed.
// Paramters : None
// Returns   : None
/*******************************************************/
void btnModePressed() {
  resetTFTlight();
  nowEvent = setEvent(selectRadioMode, now(), LOCAL_TIME);
}

/*******************************************************/
// Purpose   :
//
// Paramters :
// Returns   :
/*******************************************************/
void btnWeatherPressed() {
  resetTFTlight();
  btnSnooze.onPressed(btnWeatherExit);
  Serial.println("btnWeatherPressed()::Displaying weather screen for " + String(weatherScreenTimeout / 60) + " minutes");
  nowEvent = setEvent(paintWeatherModeScreen, now(), LOCAL_TIME);

  deleteEvents();
  if (isOn) {
    timeScreenHandle = setEvent(paintTimeModeScreen, now() + weatherScreenTimeout); // return to time screen after this time period
  } else {
    radioScreenHandle = setEvent(paintRadioScreen, now() + weatherScreenTimeout); // this needs to be every half a second to update time on screen
  }
}

void btnWeatherExit() {
  resetTFTlight();
  btnSnooze.onPressed(btnWeatherPressed);
  nowEvent = setEvent(paintTimeModeScreen, now(), LOCAL_TIME);
}
