// Libraries
#include <Arduino.h>
#include <rjConfig.h>               // program configuration
#include <settings.h>               // program settings
#include <defaults.h>               // program defaults
#include <SPI.h>                    // TFT library https://github.com/Bodmer/TFT_eSPI
#include <TFT_eSPI.h>               // Hardware-specific library
#include <WiFi.h>                   // Wifi library
#include <ezTime.h>                 // Time library
#include <TJpg_Decoder.h>           // Include the jpeg decoder library
#include <FS.h>                     // Include SPIFFS
#include <SPIFFS.h>
// #include <JSON_Decoder.h>
#include <JsonListener.h>
#include <OpenWeather.h>
#include <EasyButton.h>
#include <ESPRotary.h>
#include <weather.h>

/********* INITIALIZATION CODE *********/
// Encoders setup
ESPRotary selector = ESPRotary(SELECTOR_A_PIN, SELECTOR_B_PIN, 4);
ESPRotary volume   = ESPRotary(VOLUME_A_PIN,   VOLUME_B_PIN,   4, volume_lower_bound, volume_upper_bound);

// Button instances
EasyButton btnSelector  (SELECTOR_BTN_PIN);
EasyButton btnVolume    (VOLUME_BTN_PIN  );
EasyButton btnMode      (MODE_PIN        );
EasyButton btnStandby   (STANDBY_PIN     );
EasyButton btnSnooze    (SNOOZE_PIN      );
EasyButton btnMenu      (MENU_PIN        );

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite class needs to be invoked

Timezone myTZ;                        // ezTime instance
rjConfig con;                         // Initiate config file


/********* ENUMERATIONS ****************/
// last is not a mode... it only serves as an end to the list
// Will set mode to first in list when the end of the list is reached
enum radioModeEnum {
  internetRadio,
  dabRadio,
  fmRadio,
  // MP3,
  savedStations,
  last = savedStations
} radioMode;          // enumaration for modes

enum menus {
  ms_main,
  ms_settings,
  ms_alarms
};

enum menu_main {
  m_stations = 1,
  m_alarm,                            // alarm
  m_settings,                         // Settings
  m_main_LAST = m_settings            // last entry in the list
};

enum menu_settings {
  m_wifi = 1,                         // Wi-Fi
  m_themes,                           // Themes
  m_timeFormat,                       // Time Format
  m_screenTimeout,                    // TFT Backlight Screen Timeout
  m_weatherTimeout,                   // Weather Screen Timeout
  m_settings_LAST = m_weatherTimeout  // last entry in the list
};

enum menu_alarms {
  m_alarm1 = 1,
  m_alarm2,
  m_alarm3,
  m_alarm_LAST = m_alarm3
};

enum topbar {
  mini,   // icons only
  basic,  // date and icons
  full    // date, icons and time
};

// /********* PROTOTYPES ******************/
// //buttons
// void btnMenuPressed();
// void btnStandbyPressed();
// void btnSnoozePressed();
// void btnModePressed();
//
// //encoder_routines
// void btnSelectorPressed();
// void btnVolumePressed();
// void selectorChanged(ESPRotary& r);
// void volumeChanged(ESPRotary& r);
//
// //functions
// void deleteEvents();
// void wait(unsigned long d);
// void print_wakeup_reason();
//
// //helpers
// int getDecimal(float f, int decimal);
// void btnSelectorInterruptISR();
// void btnVolumeInterruptISR();
// void btnModeInterruptISR();
// void btnStandbyInterruptISR();
// void btnSnoozeInterruptISR();
// void btnMenuInterruptISR();
// void selector_interrupt();
// void volume_interrupt();
// void listAllFiles();
//
// //images
// bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
//
// // internet
// int Start_WiFi();
//
// // menus
// String items_main (menu_main m);
// String items_settings (menu_settings m);
// void setButtonDefaultOff();
// void setButtonDefaultOn();
//
// // radio
// void selectRadioMode();
//
// // tft
// void displayWelcomeScreen();
// void waitWelcomeScreen();
// void paintTimeModeScreen();
// void paintWeatherModeScreen();
// void paintRadioScreen();
// void topBar(topbar t = basic);
// void resetTFTlight();
// void lowerTftLED();
//
// //time
// void getInternetTime();


/********** Global variables **************/
/* MOVE TO SETTINGS FILE WHEN READY *******/
bool isOn                       = false;


/* MOVE TO SETTINGS FILE WHEN READY *******/
/******************************************/

void setup() {
  Serial.begin(115200);             // Serial monitor setup
  Serial.println(F("\r\nsetup(): begin"));

  Serial.print("C++ version: ");
  Serial.println(__cplusplus);

  // // setup TFT LED
  // ledcSetup(ledChannel, ledFreq, ledResolution);
  // ledcAttachPin(TFT_LED, ledChannel);
  //
  // // Initialise SPIFFS
  // if (!SPIFFS.begin()) {
  //   Serial.println("SPIFFS initialisation failed!");
  //   while (1) yield();              // Stay here twiddling thumbs waiting
  // }
  // Serial.println("SPIFFS Initialisation done.");
  //
  // bool font_missing = false;        // ESP32 will crash if any of the fonts are missing
  // if (SPIFFS.exists("/fonts/Roboto-Med-012.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-009.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-012.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-015.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-018.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-024.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/Roboto-Reg-036.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/RobotoM-Med-48.vlw")    == false) font_missing = true;
  // if (SPIFFS.exists("/fonts/RobotoM-Med-90.vlw")    == false) font_missing = true;
  //
  // if (font_missing) {
  //   Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
  //   while (1) yield();
  // } else {
  //   Serial.println("\r\nFonts found OK.");
  // }
  //
  // Serial.println("\n\n---- Listing files ----\n");
  // listAllFiles();
  //
  // //  bool formatted = SPIFFS.format();
  // //  if (formatted) {
  // //    Serial.println("\n\nSuccess formatting");
  // //  } else {
  // //    Serial.println("\n\nError formatting");
  // //  }
  // //
  // //  Serial.println("\n\n----Listing files after format----");
  // //  listAllFiles();
  //
  // /*
  //   First we configure the wake up source
  //   We set our ESP32 to wake up for an external trigger.
  //   There are two types for ESP32, ext0 and ext1 .
  //   ext0 uses RTC_IO to wakeup thus requires RTC peripherals
  //   to be on while ext1 uses RTC Controller so doesnt need
  //   peripherals to be powered on.
  //   Note that using internal pullups/pulldowns also requires
  //   RTC peripherals to be turned on.
  // */
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); //1 = High, 0 = Low
  //
  // // Load config for the first time
  // Serial.println(F("Loading configuration..."));
  // con.loadConfig();
  // con.saveConfig(); // to include all the new elemenets in file, if any
  //
  // // Reset config file
  // // con.resetConfig();
  //
  // // Dump config file
  // Serial.println(F("Print config file..."));
  // con.printConfig();
  //
  // //Print the wakeup reason for ESP32
  // print_wakeup_reason();
  //
  // // ezEvents debug mode - uncomment the following line
  // setDebug(DEBUG);
  //
  // //  tft.begin(0x9341);                    // TFT Display
  // tft.init();
  // tft.setRotation(1);                   // Set TFT orientation
  // tft.setSwapBytes(true);               // We need to swap the colour bytes (endianess)
  // //4.setColorDepth(16);                // 16 bit colour needed to show antialiased fonts
  // TJpgDec.setCallback(tft_output);      // The decoder must be given the exact name of the rendering function above
  //
  // displayWelcomeScreen();
  //
  // Start_WiFi();
  //
  // void setInterval(uint16_t hour = 1);  // update internet time every 1 hour
  // getCurrentWeather();
  //
  // // volume.setLimitsHandler(volume_lower_bound, volume_upper_bound);
  // volume.resetPosition();
  //
  // attachInterrupt(SELECTOR_A_PIN, selector_interrupt, CHANGE);
  // attachInterrupt(SELECTOR_B_PIN, selector_interrupt, CHANGE);
  //
  // attachInterrupt(VOLUME_A_PIN,   volume_interrupt,   CHANGE);
  // attachInterrupt(VOLUME_B_PIN,   volume_interrupt,   CHANGE);
  //
  // btnMode.begin();
  // btnStandby.begin();
  // btnSnooze.begin();
  // btnMenu.begin();
  // btnSelector.begin();
  // btnVolume.begin();
  //
  // setButtonDefaultOff();
  //
  // btnMode.enableInterrupt(btnModeInterruptISR);
  // btnStandby.enableInterrupt(btnStandbyInterruptISR);
  // btnSnooze.enableInterrupt(btnSnoozeInterruptISR);
  // btnMenu.enableInterrupt(btnMenuInterruptISR);
  // btnSelector.enableInterrupt(btnSelectorInterruptISR);
  // btnVolume.enableInterrupt(btnVolumeInterruptISR);
  //
  // resetAlarms();
  //
  // waitWelcomeScreen();
  //
  // paintTimeModeScreen();
  //
  // Serial.println(F("setup(): ready"));
}

void loop() {
  //events();
}
