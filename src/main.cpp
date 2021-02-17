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
#include <TJpg_Decoder.h>           // Include the jpeg decoder library
#include <FS.h>                     // Include SPIFFS
#include <SPIFFS.h>
// #include <JSON_Decoder.h>
#include <JsonListener.h>
#include <OpenWeather.h>
#include <EasyButton.h>
#include <ESPRotary.h>

extern void setButtonDefaultOff();

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

void setup() {
  Serial.begin(115200);             // Serial monitor setup
  Serial.println(F("\r\nsetup(): begin"));

  Serial.print("C++ version: ");
  Serial.println(__cplusplus);

  // setup TFT LED
  ledcSetup(ledChannel, ledFreq, ledResolution);
  ledcAttachPin(TFT_LED, ledChannel);

  // Initialise SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();              // Stay here twiddling thumbs waiting
  }
  Serial.println("SPIFFS Initialisation done.");

  bool font_missing = false;        // ESP32 will crash if any of the fonts are missing
  if (SPIFFS.exists("/fonts/Roboto-Med-012.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-009.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-012.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-015.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-018.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-024.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/Roboto-Reg-036.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/RobotoM-Med-48.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/RobotoM-Med-90.vlw")    == false) font_missing = true;

  if (font_missing) {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1) yield();
  } else {
    Serial.println("\r\nFonts found OK.");
  }

  Serial.println("\n\n---- Listing files ----\n");
  listAllFiles();

  //  bool formatted = SPIFFS.format();
  //  if (formatted) {
  //    Serial.println("\n\nSuccess formatting");
  //  } else {
  //    Serial.println("\n\nError formatting");
  //  }
  //
  //  Serial.println("\n\n----Listing files after format----");
  //  listAllFiles();

  /*
    First we configure the wake up source
    We set our ESP32 to wake up for an external trigger.
    There are two types for ESP32, ext0 and ext1 .
    ext0 uses RTC_IO to wakeup thus requires RTC peripherals
    to be on while ext1 uses RTC Controller so doesnt need
    peripherals to be powered on.
    Note that using internal pullups/pulldowns also requires
    RTC peripherals to be turned on.
  */
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); //1 = High, 0 = Low

  // Load config for the first time
  Serial.println(F("Loading configuration..."));
  con.loadConfig();
  con.saveConfig(); // to include all the new elemenets in file, if any

  // Reset config file
  // con.resetConfig();

  // Dump config file
  Serial.println(F("Print config file..."));
  con.printConfig();

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  // ezEvents debug mode - uncomment the following line
  setDebug(DEBUG);

  // tft.begin(0x9341);                    // TFT Display
  tft.init();
  tft.setRotation(1);                   // Set TFT orientation
  tft.setSwapBytes(true);               // We need to swap the colour bytes (endianess)
  //4.setColorDepth(16);                // 16 bit colour needed to show antialiased fonts
  TJpgDec.setCallback(tft_output);      // The decoder must be given the exact name of the rendering function above

  displayWelcomeScreen();

  Start_WiFi();

  void setInterval(uint16_t hour = 1);  // update internet time every 1 hour
  getCurrentWeather();

  volume.resetPosition();

  attachInterrupt(SELECTOR_A_PIN, selector_interrupt, CHANGE);
  attachInterrupt(SELECTOR_B_PIN, selector_interrupt, CHANGE);

  attachInterrupt(VOLUME_A_PIN,   volume_interrupt,   CHANGE);
  attachInterrupt(VOLUME_B_PIN,   volume_interrupt,   CHANGE);

  btnMode.begin();
  btnStandby.begin();
  btnSnooze.begin();
  btnMenu.begin();
  btnSelector.begin();
  btnVolume.begin();

  setButtonDefaultOff();

  btnMode.enableInterrupt(btnModeInterruptISR);
  btnStandby.enableInterrupt(btnStandbyInterruptISR);
  btnSnooze.enableInterrupt(btnSnoozeInterruptISR);
  btnMenu.enableInterrupt(btnMenuInterruptISR);
  btnSelector.enableInterrupt(btnSelectorInterruptISR);
  btnVolume.enableInterrupt(btnVolumeInterruptISR);

  resetAlarms();

  waitWelcomeScreen();

  paintTimeModeScreen();

  Serial.println(F("setup(): ready"));
}

void loop() {
  events();
}
