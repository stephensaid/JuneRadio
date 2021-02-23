#include "tft.h"

unsigned long timeWelcomeScreen;

/*******************************************************/
// Purpose   : Displays a welcome screen when power up
//             for the first time while connecting to
//             internet, syncing time, etc.
// Paramters : None
// Returns   : None
/*******************************************************/
void displayWelcomeScreen() {
  Serial.println("\ndisplayWelcomeScreen():: Displaying June One logo welcome screen...");
  resetTFTlight();
  TJpgDec.drawFsJpg(0, 0, "/juneradio.jpg"); // display welcome logo
  timeWelcomeScreen = millis();
}

/*******************************************************/
// Purpose   : Waiting time for welcome screen while
//           : we load the required items
// Paramters : None
// Returns   : None
/*******************************************************/
void waitWelcomeScreen() {
  while ( millis() < timeWelcomeScreen + 5000 ) yield();
  Serial.println("\nwaitWelcomeScreen():: Welcome screen ready");
  clearScreen();
}

/***************************************************************/
// Purpose   : Refreshes Time Mode Screen - updates time, etc
// Paramters : None
// Returns   : None
/***************************************************************/
void paintTimeModeScreen() {
  Serial.println("\npaintTimeModeScreen():: Displaying clock...");

  setEvent(paintTimeModeScreen, now() + 5); // update time screen every 5 seconds

  topBar(mini);
  int xpos, ypos, arrowXpos;

  spr.createSprite(320, 125);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);

  if (con.element.TIME_FORMAT == 1) {
    spr.loadFont(FM090M);
    spr.setTextDatum(TR_DATUM);
    spr.drawString(myTZ.dateTime("h"), (320/2) - 16, 10);

    spr.setTextDatum(TL_DATUM);
    spr.drawString(myTZ.dateTime("i"), (320/2) + 13, 10);
    spr.unloadFont();

    spr.setTextDatum(TL_DATUM);
    spr.loadFont(F018R);
    spr.drawString(myTZ.dateTime("A"), 285, 65);
    spr.unloadFont();
  } else {
    spr.loadFont(FM090M);
    spr.setTextDatum(TR_DATUM);
    spr.drawString(myTZ.dateTime("H"), (320/2) - 8, 10);

    spr.setTextDatum(TL_DATUM);
    spr.drawString(myTZ.dateTime("i"), (320/2) + 8, 10);
    spr.unloadFont();
  }

  spr.fillCircle(160, 30, 6, con.element.FG_COLOUR);       // :
  spr.fillCircle(160, 60, 6, con.element.FG_COLOUR);

  spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
  spr.setTextDatum(TC_DATUM);
  spr.loadFont(F018R);
  spr.drawString(myTZ.dateTime("l, j F Y"), 320 / 2, 95);
  spr.unloadFont();

  spr.pushSprite(0, 20);
  spr.deleteSprite();

  spr.createSprite(220, 95);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextDatum(TL_DATUM);
  ///  WEATHER  ///////////////////////////////////////////////////
  // check if we have weather data
  // If we do not have it or it is stale, display message

  time_t weatherTime = currentWeather->dt;
  time_t staleWeatherTime = UTC.now() - (60 * 60);

  if (weatherTime > staleWeatherTime ) {
    // curTemp = String(currentWeather->temp);
    String whole = String((int)(currentWeather->temp));
    String decimal = String(getDecimal(currentWeather->temp, 1));

    spr.loadFont(F018R);
    spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
    spr.drawString(currentWeather->description, 17, 10);                                // WEATHER HEADINGS ex. scattered clouds
    spr.unloadFont();

    spr.loadFont(F036R);
    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    xpos = 30;
    ypos = 45;
    spr.drawString(whole, xpos, ypos);                                                    // TEMPERATURE INTEGER
    xpos += spr.textWidth(whole);
    spr.unloadFont();

    spr.loadFont(F018R);
    ypos = 59;
    spr.drawString("." + decimal, xpos+1, ypos);                                            // TEMPERATURE DECIMAL
    xpos += spr.textWidth("." + decimal) - 10;
    ypos = 38;
    spr.drawString("°C", xpos, ypos);                                                     // DEGREES CELCIUS
    spr.unloadFont();

    float curWindSpeed = currentWeather->wind_speed * 3.6;
    whole = String((int)curWindSpeed);

    (curWindSpeed >= 10) ? decimal = "" : decimal = String(getDecimal(curWindSpeed, 1));
    if (decimal = "0") decimal = "";

    spr.loadFont(F036R);
    xpos = 105; ypos = 45;
    spr.drawString(whole, xpos, ypos);                                          // WIND SPEED
    float windSpeedWholeWidth = spr.textWidth(whole);
    xpos += windSpeedWholeWidth;
    spr.unloadFont();

    ypos = 59;
    if (decimal != "" ) {
      spr.loadFont(F018R);
      spr.drawString("." + decimal, xpos, ypos);
      xpos += spr.textWidth(decimal);
      spr.unloadFont();                                                   // WIND SPEED DECIMAL
    }

    spr.loadFont(F015R);
    xpos += 5;
    spr.drawString("km/h", xpos, ypos + 1);                                 // KM/H
    spr.unloadFont();

    String windDirection = degToCompass(currentWeather->wind_deg);
    xpos = 105 + windSpeedWholeWidth + 5;
    spr.loadFont(F018R);
    spr.drawString(windDirection, xpos, 39);                             // WIND DIRECTION TEXT
    xpos += spr.textWidth(windDirection) + 18;
    spr.unloadFont();

    ypos = 42;
    arrowXpos = xpos;

  } else {
    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    spr.loadFont(F015R);
    spr.drawString("No weather data available", 20, 45);
    spr.unloadFont();
  }

  spr.pushSprite(0, 145);
  spr.deleteSprite();

  if (weatherTime > staleWeatherTime ) {
    drawArrow(arrowXpos, 187, 20, 13, (int32_t)currentWeather->wind_deg, con.element.FG_COLOUR);

    // icons
    String bg;
    con.element.BG_COLOUR == TFT_DKGRAY ? bg = "blk" : bg = "wht";
    TJpgDec.drawFsJpg( 10, 190, "/icons/temp-" + bg + ".jpg");
  }

  spr.createSprite(100, 95);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextDatum(TL_DATUM);
  ///  ALARM  ///////////////////////////////////////////////////                               // ALARM

  if (((con.element.ALARM_1_HOUR >= 0) && (con.element.ALARM_1_HOUR <= 24)) ||
      ((con.element.ALARM_2_HOUR >= 0) && (con.element.ALARM_2_HOUR <= 24)) ||
      ((con.element.ALARM_3_HOUR >= 0) && (con.element.ALARM_3_HOUR <= 24))
     ) {

    // static time_t prev_next_alarm;
    time_t cur_next_alarm, alarm1 = (time_t)(0), alarm2 = (time_t)(0), alarm3 = (time_t)(0);

    if ((con.element.ALARM_1_HOUR >= 0) && (con.element.ALARM_1_HOUR <= 24)) {
      alarm1 = makeTime(con.element.ALARM_1_HOUR, con.element.ALARM_1_MINUTE, 0, day(), month(), year());
      if (now() >= alarm1) alarm1 += 24 * 3600;
    }

    if ((con.element.ALARM_2_HOUR >= 0) && (con.element.ALARM_2_HOUR <= 24)) {
      alarm2 = makeTime(con.element.ALARM_2_HOUR, con.element.ALARM_2_MINUTE, 0, day(), month(), year());
      if (now() >= alarm2) alarm2 += 24 * 3600;
    }

    if ((con.element.ALARM_3_HOUR >= 0) && (con.element.ALARM_3_HOUR <= 24)) {
      alarm3 = makeTime(con.element.ALARM_3_HOUR, con.element.ALARM_3_MINUTE, 0, day(), month(), year());
      if (now() >= alarm3) alarm3 += 24 * 3600;
    }

    if (alarm1 > 0) cur_next_alarm = alarm1;
    if (alarm2 > 0) {
      if (cur_next_alarm > 0) {
        cur_next_alarm = min(alarm2, cur_next_alarm);
      } else cur_next_alarm = alarm2;
    }
    if (alarm3 > 0) {
      if (cur_next_alarm > 0) {
        cur_next_alarm = min(alarm3, cur_next_alarm);
      } else cur_next_alarm = alarm3;
    }

    // Serial.print("Alarm 1    : "); Serial.println(myTZ.dateTime(alarm1));
    // Serial.print("Alarm 2    : "); Serial.println(myTZ.dateTime(alarm2));
    // Serial.print("Alarm 3    : "); Serial.println(myTZ.dateTime(alarm3));
    // Serial.print("Next alarm : "); Serial.println(myTZ.dateTime(cur_next_alarm));

    spr.loadFont(F018R);                                                        // Alarm Headings
    spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
    spr.drawString("Next Alarm", 5, 10);
    spr.unloadFont();

    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    xpos = 5; ypos = 45;

    if (con.element.TIME_FORMAT == 1) {
      spr.loadFont(F036R);
      spr.drawString(myTZ.dateTime(cur_next_alarm, "g:i"), xpos, ypos);
      int alarmTimeWidth = spr.textWidth(myTZ.dateTime(cur_next_alarm, "g:i"));
      spr.unloadFont();

      spr.loadFont(F015R);
      spr.drawString(myTZ.dateTime(cur_next_alarm, "A"), xpos + alarmTimeWidth + 3, ypos + 15);
      spr.unloadFont();
    } else {
      spr.loadFont(F036R);
      spr.drawString(myTZ.dateTime(cur_next_alarm, "H:i"), xpos, ypos);
      spr.unloadFont();
    }

  } else {
    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    spr.loadFont(F015R);
    spr.drawString("Alarm", 15, 45);                                          // Alarm not set
    spr.drawString("not set", 15, 65);
    spr.unloadFont();
  }

  spr.pushSprite(215, 145);
  spr.deleteSprite();

  if (DevMode) grid(10);
}


/*******************************************************/
// Purpose   : Paint weather screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintWeatherModeScreen() {
  // weatherScreenHandle = setEvent(paintWeatherModeScreen, now() + 5);  // this needs to be every half a second to update time and weather info on screen
  setEvent(paintWeatherModeScreen, now() + 5);  // this needs to be every half a second to update time and weather info on screen

  topBar(full);

  // fill in radio screen details here
  spr.createSprite(320, 220);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F036R);
  spr.drawString("Weather Screen", 30, 50);
  spr.unloadFont();
  spr.pushSprite(0, 20);
  spr.deleteSprite();

  /***********************************************/
  /* TEMP MEASURE   TO EXIT   ********************/
  return;
  /***********************************************/

  static bool firstTime = true;
  if ( !firstTime )
    if ( !minuteChanged() ) return;         // do nothing if time has not changed
  firstTime = false;


  // *************************************************
  // if weather hasn't changed, then exit this routine
  // *************************************************

  tft.fillScreen(con.element.BG_COLOUR);

  Serial.println("paintWeatherModeScreen():: Displaying weather screen...");

  // show last updated timestamp of weather forecast
  Serial.print("paintWeatherModeScreen():: Last weather forecast updated: ");
  Serial.println( dateTime(currentWeather->dt, "g:i A") );

  spr.loadFont(F036R);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  tft.setCursor(85, 92);
  spr.printToSprite("Weather");
  spr.unloadFont();
}

/*******************************************************/
// Purpose   : Radio screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintRadioScreen() {
  // deleteEvents();
  Serial.println("\npaintRadioScreen():: Updating radio screen");
  // radioScreenHandle = setEvent(paintRadioScreen, now() + 5);   // this needs to be every half a second to update time on screen
  setEvent(paintRadioScreen, now() + 5);   // this needs to be every half a second to update time on screen
  topBar(basic);

  // fill in radio screen details here
  spr.createSprite(320, 100);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F036R);
  spr.setTextDatum(TC_DATUM);
  spr.drawString("Radio Screen", 320/2, 50);
  spr.unloadFont();
  spr.pushSprite(0, 20);
  spr.deleteSprite();
}

/*******************************************************/
// Purpose   : Paint topbar on screen
// Paramters : t: topbar enum
//           : mini  - icons only
//           : basic - icons and date  (Default)
//           : full  - icons, time and date
// Returns   : None
/*******************************************************/
void topBar(topbar t) {
  int xpos = 320 - 10,
      ypos = 5;
  String curTime = "";

  // if ( WiFi.status() != WL_CONNECTED ) Start_WiFi();
  // shouldn't put Start_Wifi here as it is called very frequently to update time
  // maybe we can fire an event here, like:
  static time_t w;
  if ( WiFi.status() != WL_CONNECTED )  {
    Serial.print("now(): "); Serial.print(now());
    Serial.printf("  w: %s\n", ctime(&w) );
    if (  (now() - w) >= 200 ) {
      setEvent(Start_WiFi, now() + 1);
      w = now();
    }
  }

  spr.createSprite(320, 20);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F012R);

  if ( t == full ) {
    // update time
    if ( con.element.TIME_FORMAT == 1 ) {
      curTime = myTZ.dateTime("h:i A");
    } else {
      curTime = myTZ.dateTime("H:i");
    }
    xpos = xpos - spr.textWidth(curTime);
    spr.drawString(curTime, xpos, ypos);
    xpos -= 10;
  }

  if (t == basic || t == full)
    spr.drawString(myTZ.dateTime("l, j F Y"), 10, 5);

  spr.unloadFont();
  spr.pushSprite(0, 0);
  spr.deleteSprite();

  int icons_xpos = xpos;
  icons_xpos -= 16;
  ypos = 2;
  String wifi_icon;
  if ( WiFi.status() == WL_CONNECTED ) {
    con.element.BG_COLOUR == TFT_DKGRAY ? wifi_icon = "/icons/wifi_black.jpg" : wifi_icon = "/icons/wifi_white.jpg";
  } else {
    con.element.BG_COLOUR == TFT_DKGRAY ? wifi_icon = "/icons/wifi_off_black.jpg" : wifi_icon = "/icons/wifi_off_white.jpg";
  }
  TJpgDec.drawFsJpg(icons_xpos, ypos, wifi_icon);
}

/*******************************************************/
// Purpose   : paint full topbar only and keep it uptodate
//           : including date, icons and time
// Paramters : None
// Returns   : None
/*******************************************************/
void paintTopbar() {
  // deleteEvents();
  // topbarHandle = setEvent(paintTopbar, now() + 5); // this needs to be every half a second to update time on screen
  // topBar(full);
}

/*******************************************************/
// Purpose   : Reset timer for TFT before it LED
//             brightness is turned to low
// Paramters : None
// Returns   : None
/*******************************************************/
void resetTFTlight() {
  // setEvent(lowerTftLED, now() + tftDelay );
  ledcWrite(ledChannel, 255);
}


/*******************************************************/
// Purpose   : Turns TFT LED to low
// Paramters : None
// Returns   : None
/*******************************************************/
void lowerTftLED() {
  // ledcWrite(ledChannel, tftSleepBrightness);
}
