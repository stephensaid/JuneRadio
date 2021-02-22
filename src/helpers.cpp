#include <helpers.h>

extern rjConfig con;
extern TFT_eSPI tft;
extern EasyButton btnSelector;
extern EasyButton btnVolume;
extern EasyButton btnMode;
extern EasyButton btnStandby;
extern EasyButton btnSnooze;
extern EasyButton btnMenu;
extern ESPRotary selector;
extern ESPRotary volume;
extern Timezone myTZ;
extern bool radioIsOn;

extern void setButtonDefaultOff();
extern void setButtonDefaultOn();
extern void paintTimeModeScreen();
extern void paintWeatherModeScreen();
extern void paintRadioScreen();
extern void paintTopbar();

/*******************************************************/
// Purpose   : Draws knob for menus
// Paramters : None
// Returns   : None
/*******************************************************/
void drawKnob() {
  int ypos = 145;
  for (int inc = 0; inc <= 60; inc++ ) {
    unsigned int col;
    if ( (inc == 18) || (inc >= 29 && inc <= 31) || (inc == 41) ) {
      col = con.element.BG_COLOUR;
    } else col = con.element.HIGHLIGHT_COLOUR;
    fillArc(0, ypos, inc * 3, 1, 65, 65, 10, col);
  }
  tft.fillCircle(-1, ypos, 50, con.element.HIGHLIGHT_COLOUR);
  tft.fillCircle(-1, ypos, 27, con.element.BG_COLOUR);
}

/****************************************************************************/
// Purpose   : Draws a triangle/arrow and rotates it as required.
// Paramters : xpos [center x], ypos [center y], h [height], w [width]
//           : r [angle of rotation], color
// Returns   : None
/****************************************************************************/
void drawArrow( int32_t xpos, int32_t ypos, int32_t h, int32_t w, int32_t r, uint32_t color) {
  int32_t x0, y0, x1, y1, x2, y2;
  float degToRad = 0.0174533;
  x0 = xpos + (w / 2);
  y0 = ypos + (h / 2);
  x1 = xpos - (w / 2);
  y1 = ypos + (h / 2);
  x2 = xpos;
  y2 = ypos - (h / 2);

  if ( r > 360 || r < -360 ) r = 0;
  if ( r != 0 ) {
    int32_t _x0 = cos(r * degToRad) * (x0 - xpos) - sin(r * degToRad) * (y0 - ypos) + xpos;
    int32_t _y0 = sin(r * degToRad) * (x0 - xpos) + cos(r * degToRad) * (y0 - ypos) + ypos;
    int32_t _x1 = cos(r * degToRad) * (x1 - xpos) - sin(r * degToRad) * (y1 - ypos) + xpos;
    int32_t _y1 = sin(r * degToRad) * (x1 - xpos) + cos(r * degToRad) * (y1 - ypos) + ypos;
    int32_t _x2 = cos(r * degToRad) * (x2 - xpos) - sin(r * degToRad) * (y2 - ypos) + xpos;
    int32_t _y2 = sin(r * degToRad) * (x2 - xpos) + cos(r * degToRad) * (y2 - ypos) + ypos;

    x0 = _x0;
    y0 = _y0;
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
  }

  tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}


/****************************************************************************/
// Purpose   : Draw a circular or elliptical arc with a defined thickness
// Paramters : x,y == coords of centre of arc
//           : start_angle = 0 - 359
//           : seg_count = number of 6 degree segments to draw (60 => 360 degree arc)
//           : rx = x axis outer radius
//           : ry = y axis outer radius
//           : w  = width (thickness) of arc in pixels
//           : colour = 16 bit colour value
//           : Note if rx and ry are the same then an arc of a circle is drawn
// Returns   : int
/****************************************************************************/
int fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour) {
  byte seg = 6; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
  return -1;
}

/****************************************************************************/
// Purpose   : Returns Compass rose mark from degrees
// Paramters : float - degree (ex. 90 degrees returns E for East)
// Returns   : String
/****************************************************************************/
String degToCompass (float deg) {
  String directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
  int val = int((deg / 22.5) + .5);
  return directions[(val % 16)];
}

/****************************************************************************/
// Purpose   : routine to generate a square grid on screen x pixels per side
// Paramters : gridspacing (size of grid)
// Returns   : None
/****************************************************************************/
void grid (int gridspacing) {
  tft.setCursor(0, 0);
  int HScreenMax = tft.width(); // manually ammended, could be included in function as a parameter
  int VScreenMax = tft.height(); // manually ammended, could be included in function as a parameter
  int c = 0;
  tft.setTextColor(con.element.FG_COLOUR, TFT_MAGENTA);

  // vertical lines
  for (int i = 0; i < HScreenMax + 1; i += gridspacing) {
    if (c == 50) {
      tft.drawLine  (i, 0, i, VScreenMax - 1, TFT_YELLOW);
      tft.drawNumber(i, i, VScreenMax + 1, 1);
      c = 0;
    } else {
      // tft.drawLine(i, 0, i, VScreenMax - 1, con.element.FG_COLOUR);
      tft.drawLine(i, 0, i, VScreenMax - 1, TFT_CYAN);
    }
    c += gridspacing;
  } // End if

  // horizontal lines
  for (int i = 0; i < VScreenMax + 1; i += gridspacing) {
    if (c == 50) {
      tft.drawLine  (0, i, HScreenMax - 1, i, TFT_YELLOW);
      tft.drawNumber(i, HScreenMax - 20, i, 1);
      c = 0;
    } else {
      // tft.drawLine(0, i, HScreenMax - 1, i, con.element.FG_COLOUR);
      tft.drawLine(0, i, HScreenMax - 1, i, TFT_CYAN);
    }
    c += gridspacing;
  } // End if
}


/*******************************************************/
// Purpose   : Returns the part after deciaml point as integer
// Paramters : float f - number with decimal places
//           : int decimals - number of decimal places to return
// Returns   : int - decimal places as integer
/*******************************************************/
int getDecimal(float f, int decimal) {
  String result;
  int whole = (int)(f);
  float temp = f - whole;

  result = String(temp, decimal);

  if (result.charAt(0) == '-' ) {
    result = result.substring(3);
  } else {
    result = result.substring(2);
  }

  while (result.endsWith("0")) {
    result.remove(result.length() - 1, 1);
  }

  return result.toInt();
}


/****************************************************************************/
// Purpose   : Resets alarms and set events
// Paramters : None
// Returns   : None
/****************************************************************************/
void resetAlarms() {
  deleteEvent(alarm1Handle);
  deleteEvent(alarm2Handle);
  deleteEvent(alarm3Handle);

  if ((con.element.ALARM_1_HOUR >= 0) && (con.element.ALARM_1_HOUR <= 24)) {
    time_t alarm1 = makeTime(con.element.ALARM_1_HOUR, con.element.ALARM_1_MINUTE, 0, day(), month(), year());
    if (now() >= alarm1) alarm1 += 24 * 3600;
    alarm1Handle = setEvent(setOffAlarm, alarm1, LOCAL_TIME);
  }

  if ((con.element.ALARM_2_HOUR >= 0) && (con.element.ALARM_2_HOUR <= 24)) {
    time_t alarm2 = makeTime(con.element.ALARM_2_HOUR, con.element.ALARM_2_MINUTE, 0, day(), month(), year());
    if (now() >= alarm2) alarm2 += 24 * 3600;
    alarm2Handle = setEvent(setOffAlarm, alarm2, LOCAL_TIME);
  }

  if ((con.element.ALARM_3_HOUR >= 0) && (con.element.ALARM_3_HOUR <= 24)) {
    time_t alarm3 = makeTime(con.element.ALARM_3_HOUR, con.element.ALARM_3_MINUTE, 0, day(), month(), year());
    if (now() >= alarm3) alarm3 += 24 * 3600;
    alarm3Handle = setEvent(setOffAlarm, alarm3, LOCAL_TIME);
  }
}

void setOffAlarm() {
  Serial.println("setOffAlarm(): Alarm set off.");

  // set off alarm here

  btnSnooze.onPressed(snoozeAlarm);
  btnStandby.onPressed(stopAlarm);
  resetAlarms();
}

void stopAlarm() {
  Serial.println("stopAlarm() Alarm stopped.");

  // stop alarm here
  radioIsOn ? setButtonDefaultOn() : setButtonDefaultOff();
}

void snoozeAlarm() {
  Serial.println("Alarm snoozed");

  // stop alarm here

  time_t snooze = now() + 10 * 60;
  setEvent(setOffAlarm, snooze);
  radioIsOn ? setButtonDefaultOn() : setButtonDefaultOff();
}

/*******************************************************/
// Purpose   : button and encoders interrupts
// Paramters : None
// Returns   : None
/*******************************************************/
void selector_interrupt() {
  selector.loop();
}

void volume_interrupt() {
  volume.loop();
}

void btnSelectorInterruptISR() {
  btnSelector.read();
}

void btnVolumeInterruptISR() {
  btnVolume.read();
}

void btnModeInterruptISR() {
  btnMode.read();
}

void btnStandbyInterruptISR() {
  btnStandby.read();
}

void btnSnoozeInterruptISR() {
  btnSnooze.read();
}

void btnMenuInterruptISR() {
  btnMenu.read();
}


/*******************************************************/
// Purpose   : list all files in SPIFFS to serial
// Paramters : None
// Returns   : None
/*******************************************************/
void listAllFiles(){
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }
}

/*******************************************************/
// Purpose   : Deletes events (ezTime Events)
// Paramters : None
// Returns   : None
/*******************************************************/
void deleteEvents() {
  Serial.println("\ndeleteEvents():: Deleting events\n");
  // deleteEvent(timeScreenHandle);
  // deleteEvent(weatherScreenHandle);
  // deleteEvent(radioScreenHandle);
  // deleteEvent(topbarHandle);

  deleteEvent(paintTimeModeScreen);
  deleteEvent(paintWeatherModeScreen);
  deleteEvent(paintRadioScreen);
  deleteEvent(paintTopbar);

  delay(30);
}

/*******************************************************/
// Purpose   : An alternative for delay function
// Paramters : Number of milli seconds to wait
// Returns   : None
/*******************************************************/
void wait(unsigned long d) {
  unsigned long t = millis() + d;
  while ( millis() < t ) yield ();
}

/*******************************************************/
// Purpose   : Prints to serial monitor the reason for
//           : wakeup of ESP32 controller
// Paramters : None
// Returns   : None
/*******************************************************/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

/****************************************************************************/
// Purpose   : Function will be called during decoding of the jpeg file to
//             render each block to the TFT.  If you use a different TFT
//             library you will need to adapt this function to suit.
// Paramters :
// Returns   : Boolean [1 or 0]
/****************************************************************************/
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

/*************************************************************/
// Purpose   :  Routine to connect to Wifi
//              Will attempt 10 times to connect to wifi
// Paramters :  None
// Returns   :  -5 if it fails and 1 if successful
// Reference : https://www.arduino.cc/en/Reference/WiFiStatus
/*************************************************************/
void Start_WiFi() {
  String wifiSSID     = con.element.WIFI_SSID;
  String wifiPassword = con.element.WIFI_PASSWORD;

  if ( wifiSSID == "" ) {
    Serial.println("\r\nStart_WiFi::Wi-Fi failed to connect. SSID not set in config file.");
    // return -5;
    return;
  }

  Serial.printf("\r\nStart_WiFi::Connecting to %s.\n", wifiSSID.c_str() );
  int connAttempts = 0;

  while ( WiFi.status() != WL_CONNECTED ) {
    WiFi.begin( wifiSSID.c_str(), wifiPassword.c_str() );
    unsigned long t = millis();
    while (millis() < t + 500) { }
    Serial.print(".");
    if (connAttempts > 4) {
      Serial.println( "\nStart_WiFi::Wi-Fi failed to connect." );
      // return -5;
      return;
    }
    connAttempts++;
  }

  Serial.print( "\nStart_WiFi::Wi-Fi Connected. IP: " );
  Serial.println(WiFi.localIP());
  getInternetTime();
  // return 1;
}

/*************************************************************/
// Purpose   :  Gets internet time (ezTime)
// Paramters :  None
// Returns   :  None
// Reference :
/*************************************************************/
void getInternetTime() {
  if ( WiFi.status() == WL_CONNECTED ) {
    waitForSync();
    // myTZ.setLocation("mt");
    myTZ.setPosix(LOCALTZ_POSIX);
    Serial.print("\ngetInternetTime()::Updated internet time: ");
    Serial.println(myTZ.dateTime());
    myTZ.setDefault();
  } else {
    Serial.println("\ngetInternetTime()::Set time using compileTime.");
    myTZ.setPosix(LOCALTZ_POSIX);
    myTZ.setTime(compileTime());
  }
}

/*************************************************************/
// Purpose   :  Volume on left or right rotation
// Paramters :  None
// Returns   :  None
// Reference : https://github.com/LennartHennigs/ESPRotary/blob/master/examples/RangedCounter/RangedCounter.ino
/*************************************************************/
void showDirection(ESPRotary& volume) {
  Serial.println(volume.directionToString(volume.getDirection()));
}

void clearScreen() {
  tft.fillScreen(con.element.BG_COLOUR);
}
