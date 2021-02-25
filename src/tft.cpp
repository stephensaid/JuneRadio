#include "tft.h"

unsigned long timeWelcomeScreen;
bool redraw = false;

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

  setEvent(paintTimeModeScreen, now() + 1);
  int xpos, ypos;
  time_t cur_next_alarm = 0;

  topBar(mini);

  ///  ALARM  ///////////////////////////////////////////////////                               // ALARM

  if (((con.element.ALARM_1_HOUR >= 0) && (con.element.ALARM_1_HOUR <= 24)) ||
      ((con.element.ALARM_2_HOUR >= 0) && (con.element.ALARM_2_HOUR <= 24)) ||
      ((con.element.ALARM_3_HOUR >= 0) && (con.element.ALARM_3_HOUR <= 24))
     ) {

    // static time_t prev_next_alarm;
    time_t alarm1 = (time_t)(0), alarm2 = (time_t)(0), alarm3 = (time_t)(0);

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
  }


  spr.createSprite(320, 100);       // Time
  spr.fillSprite(con.element.BG_COLOUR);

  spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
  spr.setTextDatum(TL_DATUM);
  spr.loadFont(F14B);
  String dayOfWeek = myTZ.dateTime("l");
  dayOfWeek.toUpperCase();
  spr.drawString( dayOfWeek, 10, 20 );
  spr.unloadFont();

  spr.loadFont(F12L);
  spr.drawString(myTZ.dateTime("j F"), 10, 40);

  if (cur_next_alarm != 0) {
    spr.setTextDatum(TL_DATUM);
    if (con.element.TIME_FORMAT == 1) {
      spr.drawString(myTZ.dateTime(cur_next_alarm, "g:i A"), 30, 75);
    } else {
      spr.drawString(myTZ.dateTime(cur_next_alarm, "H:i"), 30, 75);
    }
  }
  spr.unloadFont();

  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);

  if (con.element.TIME_FORMAT == 1) {
    spr.loadFont(F90L);
    spr.setTextDatum(TR_DATUM);
    spr.drawString(myTZ.dateTime("g"), 193, 20);

    spr.setTextDatum(TL_DATUM);
    spr.drawString(myTZ.dateTime("i"), 215, 20);
    spr.unloadFont();

    spr.setTextDatum(TR_DATUM);
    spr.loadFont(F14B);
    spr.drawString(myTZ.dateTime("A"), 313 , 5); // AM or PM
    spr.unloadFont();
  } else {
    spr.setTextDatum(TR_DATUM);
    spr.drawString(myTZ.dateTime("G"), 195, 20);

    spr.setTextDatum(TL_DATUM);
    spr.drawString(myTZ.dateTime("i"), 215, 20);
    spr.unloadFont();
  }

  static bool dots;
  if (dots) {
    spr.loadFont(F90L);
    spr.setTextDatum(TC_DATUM);
    spr.drawString(":", 204, 15);
    dots = false;
    spr.unloadFont();
  } else {
    dots = true;
  }

  spr.pushSprite(0, 20);
  spr.deleteSprite();

  if (cur_next_alarm != 0) {
    String bg;
    con.element.BG_COLOUR == TFT_DKGRAY ? bg = "blk" : bg = "wht";
    TJpgDec.drawFsJpg( 10, 93,  "/icons/alarm-" + bg + ".jpg" );
  } else {
    spr.fillRect(10, 93, 15, 15, con.element.BG_COLOUR);
  }

  ///  WEATHER  ///////////////////////////////////////////////////
  // check if we have weather data
  // If we do not have it or it is stale, display message

  time_t weatherTime = currentWeather->dt;
  time_t staleWeatherTime = UTC.now() - (60 * 60);
  String windDirection;

  // Serial.print("weatherTime: "); Serial.println(weatherTime);
  // Serial.print("staleWeatherTime: "); Serial.println(staleWeatherTime);
  // Serial.print("weatherTime > staleWeatherTime: "); Serial.println(weatherTime > staleWeatherTime);

  static bool weatherNotice;

  if (weatherTime > staleWeatherTime ) {
    if (weatherNotice) {
       tft.fillRect( 0, 120, 320, 80, con.element.BG_COLOUR);
       weatherNotice = false;
    }
    String whole = String((int)(currentWeather->temp));
    String decimal = String(getDecimal(currentWeather->temp, 1));

    spr.createSprite(82, 80);
    spr.fillSprite(con.element.BG_COLOUR);
    spr.drawRect(0, 0, 83, 80, con.element.BG_COLOUR);

    spr.loadFont(F36L);
    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    spr.setTextDatum(BL_DATUM);
    xpos = 3;
    ypos = 50;
    spr.drawString(whole, xpos, ypos);                                                    // TEMPERATURE INTEGER
    xpos += spr.textWidth(whole);
    spr.unloadFont();

    spr.loadFont(F15L);
    spr.drawString("." + decimal + "°C", xpos + 1, ypos - 6 );                                            // TEMPERATURE DECIMAL
    spr.unloadFont();

    spr.loadFont(F12L);
    spr.setTextDatum(BL_DATUM);
    spr.drawString("Feels " + String(round(currentWeather->feels_like * 10) / 10.0, 1) + "°C", 3, 70);
    spr.unloadFont();

    spr.pushSprite(88, 120);
    spr.deleteSprite();


    spr.createSprite(149, 80);
    spr.fillSprite(con.element.BG_COLOUR);

    float curWindSpeed = currentWeather->wind_speed * 3.6; //convert to km/h
    whole = String((int)curWindSpeed);

    (curWindSpeed >= 10) ? decimal = "" : decimal = String(getDecimal(curWindSpeed, 1));
    if (decimal = "0") decimal = "";

    spr.loadFont(F36L);
    xpos = 60; ypos = 50;
    spr.setTextDatum(BL_DATUM);
    spr.drawString(whole, xpos, ypos);                                          // WIND SPEED
    float windSpeedWholeWidth = spr.textWidth(whole);
    xpos += windSpeedWholeWidth;
    spr.unloadFont();

    spr.loadFont(F15L);
    if (decimal != "" ) {
      spr.drawString("." + decimal + " km/h", xpos + 3, ypos - 5);
      xpos += spr.textWidth(decimal);
    } else {
      spr.drawString("km/h", xpos + 5, ypos - 5);                                 // KM/H
    }
    spr.unloadFont();

    spr.loadFont(F14B);
    spr.setTextDatum(BC_DATUM);
    spr.drawString(degToCompass(currentWeather->wind_deg), 35, 70);  // WIND DIRECTION TEXT
    spr.unloadFont();

    spr.loadFont(F12L );
    spr.setTextDatum(BL_DATUM);
    spr.drawString("UV Index: " + String( round(currentWeather->uvi * 10) / 10.0), 60, 69);                             // WIND DIRECTION TEXT
    spr.unloadFont();

    spr.pushSprite(171, 120);
    spr.deleteSprite();

    // static float prevWindDirection;
    // if ( prevWindDirection != currentWeather->wind_deg) {
      drawArrow(208, 150, 20, 13, (int32_t)currentWeather->wind_deg, con.element.FG_COLOUR);
    // }
    // prevWindDirection = currentWeather->wind_deg;

    if (redraw)
      tft.drawLine(170, 140, 170, (140 + 60), con.element.FG_COLOUR);

    spr.createSprite(320, 40);
    spr.fillSprite(con.element.BG_COLOUR);

    spr.loadFont(F15L);
    spr.setTextDatum(BL_DATUM);
    spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
    String weatherDesc = currentWeather->description;
    if (weatherDesc.length()>0) {
        weatherDesc[0] = std::toupper(weatherDesc[0]) ;
        for (size_t i = 1; i < weatherDesc.length(); i++)
            weatherDesc[i] = std::tolower(weatherDesc[i]);
    }
    spr.drawString(weatherDesc, 20, 20);  // WEATHER HEADINGS ex. scattered clouds
    spr.drawString("Birkirkara", 190, 20);
    spr.unloadFont();

    spr.pushSprite(0, 200);
    spr.deleteSprite();

    static String prevIcon;
    if (prevIcon != currentWeather->icon || redraw == true) {
      // weather icon
      TJpgDec.drawFsJpg( 20, 130,  getWeatherIcon( currentWeather->icon ) );

    }
    prevIcon = currentWeather->icon;

  } else {
    tft.fillRect( 0, 120, 320, 80, con.element.BG_COLOUR);
    spr.createSprite(320, 120);
    spr.fillSprite(con.element.BG_COLOUR);
    spr.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);
    spr.setTextDatum(BC_DATUM);
    spr.loadFont(F15L);
    spr.drawString("No weather data available", 320/2, 60);
    spr.unloadFont();

    spr.pushSprite(0, 120);
    spr.deleteSprite();
    weatherNotice = true;
  }

  if (GRID_ON) grid(10);
  redraw = false;
}

/*******************************************************/
// Purpose   : Paint weather screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintWeatherModeScreen() {
  Serial.println("paintWeatherModeScreen():: Displaying weather screen...");
  setEvent(paintTopbar, now() + 10);  // this needs to be every half a second to update time and weather info on screen
  // topBar(full);

  // fill in radio screen details here
  spr.createSprite(320, 140);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F36L);
  spr.setTextDatum(MC_DATUM);
  spr.drawString("Weather Screen", 320/2, 140/2);
  spr.unloadFont();
  spr.pushSprite(0, 20);
  spr.deleteSprite();

  redraw = false; // put as last line of code
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



  // show last updated timestamp of weather forecast
  Serial.print("paintWeatherModeScreen():: Last weather forecast updated: ");
  Serial.println( dateTime(currentWeather->dt, "g:i A") );

}

/*******************************************************/
// Purpose   : Radio screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintRadioScreen() {
  Serial.println("\npaintRadioScreen():: Updating radio screen");
  setEvent(paintRadioScreen, now() + 5);   // this needs to be every half a second to update time on screen
  topBar(basic);

  // fill in radio screen details here
  spr.createSprite(320, 100);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F36L);
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
  static time_t w = 0;
  if ( WiFi.status() != WL_CONNECTED )  {
    Serial.print("millis(): "); Serial.print(millis());
    Serial.print("  w: "); Serial.println(w);
    if (  (millis() - w) > 5000 ) {
      setEvent(Start_WiFi, now() + 1);
      w = millis();
    }
  }

  spr.createSprite(320, 20);
  spr.fillSprite(con.element.BG_COLOUR);
  spr.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);
  spr.loadFont(F12L);
  spr.setTextDatum(TR_DATUM);

  if ( t == full ) {
    // update time
    if ( con.element.TIME_FORMAT == 1 ) {
      curTime = myTZ.dateTime("h:i A");
    } else {
      curTime = myTZ.dateTime("H:i");
    }
    spr.drawString(curTime, xpos, ypos);
    xpos = xpos - spr.textWidth(curTime);
    xpos -= 10;
  }

  if (t == basic || t == full) {
    spr.setTextDatum(TL_DATUM);
    spr.drawString(myTZ.dateTime("l, j F Y"), 10, ypos);
  }

  spr.unloadFont();
  spr.pushSprite(0, 0);
  spr.deleteSprite();

  xpos -= 16;
  ypos = 2;
  String wifi_icon;
  if ( WiFi.status() == WL_CONNECTED ) {
    con.element.BG_COLOUR == TFT_DKGRAY ? wifi_icon = "/icons/wifi_black.jpg" : wifi_icon = "/icons/wifi_white.jpg";
  } else {
    con.element.BG_COLOUR == TFT_DKGRAY ? wifi_icon = "/icons/wifi_off_black.jpg" : wifi_icon = "/icons/wifi_off_white.jpg";
  }
  TJpgDec.drawFsJpg(xpos, ypos, wifi_icon);
}

/*******************************************************/
// Purpose   : paint full topbar only and keep it uptodate
//           : including date, icons and time
// Paramters : None
// Returns   : None
/*******************************************************/
void paintTopbar() {
  setEvent(paintTopbar, now() + 1); // this needs to be every half a second to update time on screen
  topBar(full);
}

/*******************************************************/
// Purpose   : Delete events and calls Time Screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintTimeModeScreen_reset() {
  deleteEvents();
  clearScreen();
  paintTimeModeScreen();
}

/*******************************************************/
// Purpose   : Delete events and calls Radio Screen
// Paramters : None
// Returns   : None
/*******************************************************/
void paintRadioScreen_reset() {
  deleteEvents();
  clearScreen();
  paintRadioScreen();
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

void clearScreen() {
  tft.fillScreen(con.element.BG_COLOUR);
  redraw = true;
}
