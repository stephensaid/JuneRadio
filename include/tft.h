#ifndef tft_h
#define tft_h

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <helpers.h>
#include <defaults.h>
#include <ezTime.h>
#include <weather.h>

extern OpenWeatherMapCurrentData weatherData;
extern rjConfig con;
extern TFT_eSPI tft;
extern TFT_eSprite spr;
extern Timezone myTZ;

enum topbar {
  mini,   // icons only
  basic,  // date and icons
  full    // date, icons and time
};

void displayWelcomeScreen();
void waitWelcomeScreen();
void paintTimeModeScreen();
void paintWeatherModeScreen();
void paintRadioScreen();
void topBar(topbar t = basic);
void resetTFTlight();
void lowerTftLED();

#endif //tft_h
