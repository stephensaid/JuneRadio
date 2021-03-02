#ifndef tft_h
#define tft_h

#include <Arduino.h>
#include <TFT_eSPI.h>
// #include <TJpg_Decoder.h>
#include <TFT_eFEX.h>
#include <helpers.h>
#include <defaults.h>
#include <ezTime.h>
#include <weather.h>

extern OW_current *currentWeather;
extern rjConfig con;
extern TFT_eSPI tft;
extern TFT_eSprite spr;
extern TFT_eFEX fex;
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
void paintTopbar();
void paintTimeModeScreen_reset();
void paintRadioScreen_reset();
void resetTFTlight();
void lowerTftLED();
void clearScreen();


#endif //tft_h
