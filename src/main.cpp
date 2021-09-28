#include "main.h"

void setup() {
  Serial.begin(115200);             // Serial monitor setup
  // Serial.begin(250000);             // Serial monitor setup
  Serial.println(F("\r\nsetup(): begin"));

  Serial.print("C++ version: ");
  Serial.println(__cplusplus);

  // setup TFT LED
  ledcSetup(LED_CHANNEL, LED_FREQ, LED_RESOLUTION);
  ledcAttachPin(TFT_LED, LED_CHANNEL);

  // Initialise SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();              // Stay here twiddling thumbs waiting
  }
  if (DEBUG_INFO) Serial.println("SPIFFS Initialisation done.");

  // formatSPIFFS();

  bool font_missing = false;        // ESP32 will crash if any of the fonts are missing
  if (SPIFFS.exists("/fonts/NotoSans-Light-10.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-12.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-15.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-18.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-24.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-36.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Light-90.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSans-Bold-14.vlw" )    == false) font_missing = true;

  if (font_missing) {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1) yield();
  } else {
    if (DEBUG_INFO) Serial.println("\r\nFonts found OK.");
  }

  if (DEBUG_INFO) {
    Serial.println("\n\n---- Listing files ----\n");
    listAllFiles();
  }

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
  if (DEBUG_INFO) Serial.println(F("Loading configuration..."));
  con.loadConfig();
  // Serial.println(F("\nPrint config file..."));
  // con.printConfig();
  // con.saveConfig(); // to include all the new elemenets in file, if any

  // Reset config file
  // con.resetConfig();

  // Dump config file
  if (DEBUG_INFO) Serial.println(F("\nPrint config file..."));
  con.printConfig();

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  // ezEvents debug mode - uncomment the following line
  setDebug(INFO);

  // tft.begin(0x9341);                    // TFT Display
  tft.init();
  tft.setRotation(3);                   // Set TFT orientation
  tft.setSwapBytes(true);               // We need to swap the colour bytes (endianess)
  spr.setColorDepth(16);                 // 16 bit colour needed to show antialiased fonts
  // TJpgDec.setCallback(tft_output);      // The decoder must be given the exact name of the rendering function above

  displayWelcomeScreen();

  Start_WiFi();

  void setInterval(uint16_t hour = 1);  // update internet time every 1 hour

  volume.resetPosition(); // setting volume to 0

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

  setEvent(getCurrentWeather, now() + 1 );

  Serial.println(F("setup(): ready"));
}

void loop() {
  events();
}
