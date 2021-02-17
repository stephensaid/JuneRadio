#ifndef menus_h
#define menus_h

#include <Arduino.h>
#include <ESPRotary.h>
#include <TFT_eSPI.h>
#include <tft.h>
#include <defaults.h>
#include <ezTime.h>
#include <buttons.h>

/********* ENUMERATIONS ****************/
// last is not a mode... it only serves as an end to the list
// Will set mode to first in list when the end of the list is reached
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

String items_main (menu_main m);
String items_settings (menu_settings m);
void menu( int d, boolean click = false, menus m = ms_main);
void invokeMainMenu();
void exitMainMenu();
void menuSelect();
void rotMenuBrowse(ESPRotary & r);
void menuBrowse();
void setButtonDefaultInMenu();
void setButtonDefaultOff();
void setButtonDefaultOn();
void disableControl();
void disableControl(ESPRotary & r);

#endif //menus_h
