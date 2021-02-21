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
  tft.fillScreen(con.element.BG_COLOUR);
}

/***************************************************************/
// Purpose   : Refreshes Time Mode Screen - updates time, etc
// Paramters : None
// Returns   : None
/***************************************************************/
void paintTimeModeScreen() {
  Serial.println("\npaintTimeModeScreen():: Displaying clock...");

  deleteEvents();
  timeScreenHandle = setEvent(paintTimeModeScreen, now() + 5); // update time screen every half a second

  topBar(mini);
  static String prevDate, prevAP, prevMin, prevHour, prevTemp, prevWindDir;
  static float prevWindSpeed;
  static time_t prevTime;

  String curHour, curMin, curAP, curDate, curTemp, bg, whole, decimal;
  int xpos, ypos;
  float windSpeedWholeWidth, curWindSpeed;

  time_t curTime = myTZ.now();

  if (curTime != prevTime) {

    curDate = " " + myTZ.dateTime("l, j F Y") + " ";

    if ( con.element.TIME_FORMAT == 1 ) {
      // 12 HOUR FORMAT
      curHour = myTZ.dateTime("h");
      curMin = myTZ.dateTime("i");
      myTZ.dateTime("A") == "alarmAM" ? curAP = "A" : curAP = "P";
    } else {
      // 24 HOUR FORMAT
      curHour = myTZ.dateTime("H");
      curMin = myTZ.dateTime("i");
      curAP = "";
    }

    tft.setTextPadding(20);
    tft.loadFont(fM090m);
    tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);

    if ( prevHour != curHour ) {                                            // HOUR
      tft.fillRect( 36,  30, 114, 70, con.element.BG_COLOUR);               // HOUR
      tft.drawString(curHour, 36, 30);
      prevHour = curHour;
      tft.fillCircle(160, 50, 6, con.element.FG_COLOUR);                    // :
      tft.fillCircle(160, 80, 6, con.element.FG_COLOUR);
    }

    if ( prevMin != curMin ) {                                              // MINUTES
      tft.fillRect(170,  30, 120, 70, con.element.BG_COLOUR);               // MINUTES
      tft.drawString(curMin, 180, 30);
      prevMin = curMin;
    }

    tft.unloadFont();

    if ( prevAP != curAP ) {                                                 // alarmAM - PM
      tft.fillRect(290,  75,  24, 25, con.element.BG_COLOUR);                // alarmAM - PM
      tft.loadFont(f024r);
      tft.drawString(curAP, 290, 80);
      tft.unloadFont();
      prevAP = curAP;
    }

    if ( prevDate != curDate ) {                                            // DATE
      tft.loadFont(f018r);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
      tft.drawString(curDate, ((320 - tft.textWidth(curDate)) / 2), 115);
      prevDate = curDate;
      tft.unloadFont();
    }
    prevTime = curTime;
  }


  ///  WEATHER  ///////////////////////////////////////////////////
  // check if we have weather data
  // If we do not have it or it is stale, display message

  time_t weatherTime = weatherData.observationTime;
  time_t staleWeatherTime = UTC.now() - (60 * 60);

  if (weatherTime > staleWeatherTime ) {
    curTemp = String(weatherData.temp);
    String windDirection = degToCompass(weatherData.windDeg);
    curWindSpeed = weatherData.windSpeed * 3.6;

    Serial.printf("curTemp: %s; prevTemp: %s; windDirection: %s; prevWindDir: %s; curWindSpeed: %f; prevWindSpeed: %f.\n", curTemp.c_str(), prevTemp.c_str(), windDirection.c_str(), prevWindDir.c_str(), curWindSpeed, prevWindSpeed);

    if ( prevTemp != curTemp || prevWindDir != windDirection || prevWindSpeed != curWindSpeed ) {
      whole = String((int)(weatherData.temp));
      decimal = String(getDecimal(weatherData.temp, 1));

      tft.loadFont(f018r);
      tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
      tft.fillRect( 10, 140, 200, 38, con.element.BG_COLOUR);                               // WEATHER HEADINGS
      tft.drawString(weatherData.description, 15, 155);                                            // WEATHER HEADINGS ex. scattered clouds
      tft.unloadFont();

      // icons
      tft.fillRect( 0, 190, 30, 15, con.element.BG_COLOUR);
      con.element.BG_COLOUR == TFT_DKGRAY ? bg = "blk" : bg = "wht";
      TJpgDec.drawFsJpg( 10, 190, "/icons/temp-" + bg + ".jpg");

      tft.loadFont(f036r);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
      xpos = 30;
      ypos = 190;
      tft.fillRect( xpos, 178, 70, 48, con.element.BG_COLOUR);                               // TEMPERATURE INTEGER
      tft.drawString(whole, xpos, ypos);                                                    // TEMPERATURE INTEGER
      xpos += tft.textWidth(whole);
      tft.unloadFont();

      tft.loadFont(f018r);
      ypos = 204;
      tft.drawString("." + decimal, xpos, ypos);                                            // TEMPERATURE DECIMAL

      xpos += tft.textWidth("." + decimal) - 10;
      ypos = 183;
      tft.drawString("°C", xpos, ypos);                                                     // DEGREES CELCIUS
      tft.unloadFont();

      prevTemp = curTemp;
    }

    if ( prevWindSpeed != curWindSpeed ) {
      whole = String((int)curWindSpeed);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);

      (curWindSpeed >= 10) ? decimal = "" : decimal = String(getDecimal(curWindSpeed, 1));
      if (decimal = "0") decimal = "";
      tft.fillRect(100, 178, 110, 48, con.element.BG_COLOUR);                               //WIND

      tft.loadFont(f036r);
      xpos = 105; ypos = 190;
      tft.drawString(whole, xpos, ypos);                                          // WIND SPEED
      windSpeedWholeWidth = tft.textWidth(whole);
      xpos += windSpeedWholeWidth;
      tft.unloadFont();

      ypos = 204;
      if (decimal != "" ) {
        tft.loadFont(f018r);
        tft.drawString("." + decimal, xpos, ypos);
        xpos += tft.textWidth(decimal);
        tft.unloadFont();                                                   // WIND SPEED DECIMAL
      }

      tft.loadFont(f015r);
      xpos += 5;
      tft.drawString("km/h", xpos, ypos + 1);                                 // KM/H
      tft.unloadFont();

      xpos = 105 + windSpeedWholeWidth + 5;
      tft.loadFont(f018r);
      tft.drawString(windDirection, xpos, 184);                             // WIND DIRECTION TEXT
      xpos += tft.textWidth(windDirection) + 18;
      tft.unloadFont();
      prevWindDir = windDirection;

      ypos = 187;
      tft.fillCircle(xpos, ypos, 14, con.element.BG_COLOUR);
      drawArrow(xpos, ypos, 20, 13, (int32_t)weatherData.windDeg, con.element.FG_COLOUR);
      prevWindSpeed = curWindSpeed;
    }

  } else {
    tft.fillRect(0, 135, 214, 105, con.element.BG_COLOUR);
    tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    tft.loadFont(f015r);
    tft.drawString("No weather data available", 20, 190);
    tft.unloadFont();
  }

  ///  ALARM  ///////////////////////////////////////////////////                               // ALARM

  if (((con.element.ALARM_1_HOUR >= 0) && (con.element.ALARM_1_HOUR <= 24)) ||
      ((con.element.ALARM_2_HOUR >= 0) && (con.element.ALARM_2_HOUR <= 24)) ||
      ((con.element.ALARM_3_HOUR >= 0) && (con.element.ALARM_3_HOUR <= 24))
     ) {

    static time_t prev_next_alarm;
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

    if (cur_next_alarm != prev_next_alarm) {
      Serial.print("Alarm 1    : "); Serial.println(myTZ.dateTime(alarm1));
      Serial.print("Alarm 2    : "); Serial.println(myTZ.dateTime(alarm2));
      Serial.print("Alarm 3    : "); Serial.println(myTZ.dateTime(alarm3));
      Serial.print("Next alarm : "); Serial.println(myTZ.dateTime(cur_next_alarm));

      tft.fillRect(210, 178, 110, 48, con.element.BG_COLOUR);                     // ALARM BACKGROUND
      tft.loadFont(f018r);                                                        // Alarm Headings
      tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
      tft.drawString("Next Alarm", 215, 155);
      tft.unloadFont();

      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
      tft.loadFont(f036r);
      xpos = 215; ypos = 190;

      String alarm, alarmAM = "";
      if (con.element.TIME_FORMAT == 1) {
        alarm = myTZ.dateTime(cur_next_alarm, "g:i");
        myTZ.dateTime(cur_next_alarm, "A") == "alarmAM" ? alarmAM = "A" : alarmAM = "P";
      } else {
        alarm = myTZ.dateTime(cur_next_alarm, "H:i");
        alarmAM = "";
      }

      tft.drawString(alarm, xpos, ypos);                                        // ALARM
      xpos += tft.textWidth(alarm) + 3; ypos = 204;
      tft.unloadFont();

      tft.loadFont(f018r);
      tft.drawString(alarmAM, xpos, ypos);                                            // ALARM alarmAM FM
      tft.unloadFont();

      prev_next_alarm = cur_next_alarm;
    }
  } else {
    tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    tft.loadFont(f015r);
    tft.drawString("Alarm", 235, 190);                                          // Alarm not set
    tft.drawString("not set", 235, 210);
    tft.unloadFont();
  }

  if (DevMode) grid(10);
}

/*******************************************************/
// Purpose   : Paint weather screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintWeatherModeScreen() {
  weatherScreenHandle = setEvent(paintWeatherModeScreen, now() + 30);  // this needs to be every half a second to update time and weather info on screen

  /***********************************************/
  /* TEMP MEASURE   TO REMOVE   ******************/
  return;
  /***********************************************/

  static bool firstTime = true;
  if ( !firstTime )
    if ( !minuteChanged() ) return;         // do nothing if time has not changed
  firstTime = false;

  topBar(full);

  // *************************************************
  // if weather hasn't changed, then exit this routine
  // *************************************************

  tft.fillScreen(con.element.BG_COLOUR);

  Serial.println("paintWeatherModeScreen()::Displaying weather screen...");

  // show last updated timestamp of weather forecast
  Serial.print("paintWeatherModeScreen()::Last weather forecast updated: ");
  Serial.println( lastWeatherUpdateDateTime() );

  spr.loadFont(f036r);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  tft.setCursor(85, 92);
  spr.printToSprite("Weather");
  spr.unloadFont();
}

/*******************************************************/
// Purpose   : Displays a welcome screen when power up
//             for the first time while connecting to
//             internet, syncing time, etc.
// Paramters : None
// Returns   : None
/*******************************************************/
void paintRadioScreen() {
  Serial.println("paintRadioScreen :: Updating screen");
  deleteEvents();
  radioScreenHandle = setEvent(paintRadioScreen, now() + 30);   // this needs to be every half a second to update time on screen
  topBar();
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

  spr.createSprite(320, 20);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(f015r);

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
    spr.drawString(myTZ.dateTime("j F Y"), 10, 5);

  spr.unloadFont();
  spr.pushSprite(0, 0);

  int icons_xpos = xpos;
  icons_xpos -= 16;
  ypos = 4;
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
  topBar(full);
  deleteEvents();
  topbarHandle = setEvent(paintTopbar, now() + 30); // this needs to be every half a second to update time on screen
}

/*******************************************************/
// Purpose   : Reset timer for TFT before it LED
//             brightness is turned to low
// Paramters : None
// Returns   : None
/*******************************************************/
void resetTFTlight() {
  static uint8_t tftLightHandle;
  deleteEvent(tftLightHandle);
  tftLightHandle = setEvent(lowerTftLED, now() + tftDelay );
  ledcWrite(ledChannel, 255);
}


/*******************************************************/
// Purpose   : Turns TFT LED to low
// Paramters : None
// Returns   : None
/*******************************************************/
void lowerTftLED() {
  ledcWrite(ledChannel, tftSleepBrightness);
}
