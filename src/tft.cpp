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
  timeScreenHandle = setEvent(paintTimeModeScreen, now() + 30); // update time screen every half a second

  topBar(mini);
  static String prevDate, prevAP, prevMin, prevHour, prevTemp;
  static float prevWindSpeed;

  String curHour, curMin, curAP, curDate, curTemp, bg, whole, decimal;
  int xpos, ypos;
  float stringWidth, curWindSpeed;

  curDate = " " + myTZ.dateTime("l, j F Y") + " ";

  if ( con.element.TIME_FORMAT == 1 ) {
    // 12 HOUR FORMAT
    curHour = myTZ.dateTime("h");
    curMin = myTZ.dateTime("i");
    myTZ.dateTime("A") == "AM" ? curAP = "A" : curAP = "P";
  } else {
    // 24 HOUR FORMAT
    curHour = myTZ.dateTime("H");
    curMin = myTZ.dateTime("i");
  }

  tft.setTextPadding(20);
  tft.loadFont(fM090m);
  tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);

  if ( prevHour != curHour ) {                                            // HOUR
    tft.fillRect( 36,  30, 114, 70, con.element.BG_COLOUR);                         // HOUR
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

  if ( prevAP != curAP ) {                                                 // AM - PM
    tft.fillRect(290,  75,  24, 25, con.element.BG_COLOUR);                // AM - PM
    tft.loadFont(f024r);
    tft.drawString(curAP, 290, 80);
    prevAP = curAP;
    tft.unloadFont();
  }

  if ( prevDate != curDate ) {                                            // DATE
    tft.loadFont(f018r);
    tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    tft.drawString(curDate, ((320 - tft.textWidth(curDate)) / 2), 115);
    prevDate = curDate;
    tft.unloadFont();
  }

  ///  WEATHER  ///////////////////////////////////////////////////
  // check if we have weather data
  // If we do not have it or it is stale, display message

  time_t weatherTime = weatherData.observationTime;
  time_t nowTime = UTC.now() - (60 * 60);

  // Serial.printf("observationTime: %d, full date: %s", weatherData.observationTime, ctime(&weatherTime));
  // Serial.print("nowTime:");  Serial.println(nowTime);
  // Serial.printf("nowTime        :     full date: %s", ctime(&nowTime) );

  if (weatherTime > nowTime ) {

    con.element.BG_COLOUR == TFT_DKGRAY ? bg = "blk" : bg = "wht";
    curTemp = String(weatherData.temp);

    if (prevTemp != curTemp) {
      whole = String((int)(weatherData.temp));
      decimal = String(getDecimal(weatherData.temp, 1));

      tft.loadFont(f018r);
      tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
      tft.fillRect( 10, 140, 200, 38, con.element.BG_COLOUR);                               // WEATHER HEADINGS
      tft.drawString(weatherData.description, 15, 155);                                            // WEATHER HEADINGS ex. scattered clouds
      //    tft.drawString("Wind", 110, 155);                                               // WIND HEADING
      tft.unloadFont();

      // icons
      TJpgDec.drawFsJpg( 10, 190, "/icons/temp-" + bg + ".jpg");

      tft.loadFont(f036r);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
      xpos = 30;
      ypos = 190;
      stringWidth = tft.textWidth(whole);
      tft.fillRect( 30, 178,  70, 48, con.element.BG_COLOUR);                               // TEMPERATURE INTEGER
      tft.drawString(whole, xpos, ypos);                                                    // TEMPERATURE INTEGER
      tft.unloadFont();

      tft.loadFont(f018r);
      xpos += stringWidth;
      ypos = 204;
      stringWidth = tft.textWidth("." + decimal);
      tft.drawString("." + decimal, xpos, ypos);                                            // TEMPERATURE DECIMAL

      xpos += stringWidth - 8;
      ypos = 180;
      tft.drawString("°C", xpos, ypos);                                                     // DEGREES CELCIUS
      tft.unloadFont();

      prevTemp = curTemp;
    }

    curWindSpeed = weatherData.windSpeed * 3.6;

    if ( prevWindSpeed != curWindSpeed ) {
      whole = String((int)curWindSpeed);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);

      (curWindSpeed >= 10) ? decimal = "" : decimal = String(getDecimal(curWindSpeed, 1));
      tft.fillRect(100, 178, 110, 48, con.element.BG_COLOUR);                               //WIND

      tft.loadFont(f036r);
      xpos = 110; ypos = 190;
      tft.drawString(whole, xpos, ypos);                                          // WIND SPEED
      stringWidth = tft.textWidth(whole);
      tft.unloadFont();

      ypos = 204;
      if (decimal = "0") decimal = "";
      if (decimal != "" ) {
        tft.loadFont(f018r);
        xpos += stringWidth;
        tft.drawString("." + decimal, xpos, ypos);
        tft.unloadFont();                                                   // WIND SPEED DECIMAL
      }

      tft.loadFont(f015r);
      xpos += stringWidth + 8;
      tft.drawString("km/h", xpos, ypos + 1);                                 // KM/H
      tft.unloadFont();

      prevWindSpeed = curWindSpeed;
    }

    xpos = 134;
    tft.loadFont(f024r);
    String windDirection = degToCompass(weatherData.windDeg);
    Serial.printf("Wind direction: %s which is %d wide.", windDirection.c_str(), tft.textWidth(windDirection));
    tft.drawString(windDirection, xpos, 180);  // WIND DIRECTION TEXT
    tft.unloadFont();

    xpos += tft.textWidth(windDirection) + 28 ;
    ypos = 187;

    // delay(3000);
    //
    // for (int d = 0; d < 360; d++ ) {
    //   drawArrow(xpos, ypos, 20, 13, d, con.element.FG_COLOUR);
    //   delay (10);
    //   tft.fillCircle(xpos, ypos, 14, con.element.BG_COLOUR);
    // }
    //
    // delay(3000);

    tft.fillCircle(xpos, ypos, 14, con.element.BG_COLOUR);
    drawArrow(xpos, ypos, 20, 13, (int32_t)weatherData.windDeg, con.element.FG_COLOUR);

  } else {
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

    Serial.print("Alarm 1    : "); Serial.println(myTZ.dateTime(alarm1));
    Serial.print("Alarm 2    : "); Serial.println(myTZ.dateTime(alarm2));
    Serial.print("Alarm 3    : "); Serial.println(myTZ.dateTime(alarm3));
    Serial.print("Next alarm : "); Serial.println(myTZ.dateTime(cur_next_alarm));

    // time_t cur_next_alarm = min(alarm1, min(alarm2, alarm3));

    if (cur_next_alarm != prev_next_alarm) {
      tft.fillRect(210, 178, 110, 48, con.element.BG_COLOUR);                     // ALARM BACKGROUND
      tft.loadFont(f018r);                                                        // Alarm Headings
      tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
      tft.drawString("Next Alarm", 215, 155);
      tft.unloadFont();

      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
      tft.loadFont(f036r);
      xpos = 215; ypos = 190;

      String alarm, AM = "";
      if (con.element.TIME_FORMAT == 1) {
        alarm = dateTime(cur_next_alarm, "g:i");
        myTZ.dateTime(cur_next_alarm, "A") == "AM" ? AM = "A" : AM = "P";
      } else {
        alarm = dateTime(cur_next_alarm, "H:i");
      }
      tft.drawString(alarm, xpos, ypos);                                        // ALARM
      stringWidth = tft.textWidth(alarm);
      tft.unloadFont();

      tft.loadFont(f018r);
      xpos += stringWidth + 3; ypos = 204;
      tft.drawString(AM, xpos, ypos);                                            // ALARM AM FM
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
