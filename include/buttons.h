#ifndef buttons_h
#define buttons_h

#include <Arduino.h>
#include <ezTime.h>
#include <tft.h>
#include <defaults.h>
#include <TFT_eSPI.h>
#include <tft.h>

void btnStandbyTurnOn();
void turnOn();
void btnStandbyTurnOff();
void turnOff();
void btnMenuPressed();
void btnExitMainMenu();
void btnModePressed();
void btnWeatherPressed();
void btnWeatherExit();

// void btnStandbyPressed();
// void btnSnoozePressed();


#endif //buttons_h
