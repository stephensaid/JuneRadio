#ifndef main_h
#define main_h

// Libraries
#include <Arduino.h>
#include <rjConfig.h>               // program configuration
#include <defaults.h>               // program defaults
#include <helpers.h>                // helper functions
#include <weather.h>                // weather functions
#include <tft.h>                    // tft related functions

#include <SPI.h>                    // TFT library https://github.com/Bodmer/TFT_eSPI
#include <TFT_eSPI.h>               // Hardware-specific library
#include <WiFi.h>                   // Wifi library
#include <ezTime.h>                 // Time library
// #include <TJpg_Decoder.h>           // Include the jpeg decoder library
#include <TFT_eFEX.h>               // Include the function extension library
#include <FS.h>                     // Include SPIFFS
#include <SPIFFS.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include <EasyButton.h>
#include <ESPRotary.h>

extern void setButtonDefaultOff();

/********* INITIALIZATION CODE *********/
// Encoders setup
ESPRotary selector = ESPRotary(SELECTOR_A_PIN, SELECTOR_B_PIN, 4);
ESPRotary volume   = ESPRotary(VOLUME_A_PIN,   VOLUME_B_PIN,   4, VOLUME_LOWER_BOUND, VOLUME_UPPER_BOUND);

// Button instances
EasyButton btnSelector  (SELECTOR_BTN_PIN);
EasyButton btnVolume    (VOLUME_BTN_PIN  );
EasyButton btnMode      (MODE_PIN        );
EasyButton btnStandby   (STANDBY_PIN     );
EasyButton btnSnooze    (SNOOZE_PIN      );
EasyButton btnMenu      (MENU_PIN        );

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite class needs to be invoked
TFT_eFEX    fex = TFT_eFEX(&tft);     // Create TFT_eFX object "fex" with pointer to "tft" object

Timezone myTZ;                        // ezTime instance
rjConfig con;                         // Initiate config file

#endif // end main_h
