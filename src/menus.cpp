#include "menus.h"

extern TFT_eSPI tft;
extern EasyButton btnSelector;
extern EasyButton btnVolume;
extern EasyButton btnMode;
extern EasyButton btnStandby;
extern EasyButton btnSnooze;
extern EasyButton btnMenu;
extern ESPRotary selector;
extern ESPRotary volume;

extern void paintTopbar();
extern void btnMenuPressed();
extern void btnExitMainMenu();
extern void btnModePressed();
extern void btnWeatherPressed();
extern void btnWeatherExit();
extern void btnVolumePressed();
extern void volumeChanged(ESPRotary& r);
extern void btnSelectorPressed();
extern void btnMenuSelect();
extern void selectorChanged(ESPRotary& r);
extern void callFormatSPIFFS();


int dir;

/*******************************************************/
// Purpose   : Sets Main Menu name
// Paramters : Menu item (enum)
// Returns   : String: menu name
/*******************************************************/
String items_main (menu_main m) {
  String result;
  yield();
  if ( m >= m_main_LAST || m < 1 ) return "";
  switch (m) {
    case m_stations:          result = "Saved stations";  break;
    case m_alarm:             result = "Alarms";          break;
    case m_settings:          result = "Settings";        break;
  }
  return result;
}

/*******************************************************/
// Purpose   : Sets Settings Menu name
// Paramters : Menu item (enum)
// Returns   : String: menu name
/*******************************************************/
String items_settings (menu_settings m) {
  String result;
  yield();
  switch (m) {
    case m_wifi:              result = "Wi-Fi";           break;
    case m_themes:            result = "Themes";          break;
    case m_timeFormat:        result = "Time Format";     break;
    case m_screenTimeout:     result = "Screen timeout";  break;
    case m_weatherTimeout:    result = "Weather timeout"; break;
  }
  return result;
}

/*******************************************************/
// Purpose   : Alarms Menu name
// Paramters : Menu item (enum)
// Returns   : String: menu name
/*******************************************************/
String items_alarms (menu_alarms m) {
  String result;
  yield();
  switch (m) {
    case m_alarm1:            result = "Alarm 1";         break;
    case m_alarm2:            result = "Alarm 2";         break;
    case m_alarm3:            result = "Alarm 3";         break;
  }
  return result;
}


/*******************************************************/
// Purpose   : Menu handler
// Paramters : int d          : direction
//           : boolean click  : default false
//           : menus m        : default - main menu
// Returns   : void
/*******************************************************/
void menu( int d, boolean click, menus m) {
  String strThis = "",
         strPrev = "",
         strNext = "";
  static menus curMenu;
  static int p = 0;
  yield();

  if (click == true) {
      p = 0;
      // menu item selected
      tft.fillScreen(con.element.BG_COLOUR);
      paintTopbar();
      drawKnob();
      tft.fillRect(28, 38, 200, 40, con.element.BG_COLOUR);
      tft.loadFont(F36L);
      tft.setTextColor(con.element.HIGHLIGHT_COLOUR, con.element.BG_COLOUR);

      switch (curMenu) {
        case ms_main:
          // load the menu
          Serial.print("Menu name: "); Serial.println(menu_main_name);
          tft.drawString(menu_main_name, 30, 40);
          tft.unloadFont();
          menu(0, false, ms_main);
          break;
        case ms_settings:
          // load the menu
          Serial.print("Menu name: "); Serial.println(menu_settings_name);
          tft.drawString(menu_settings_name, 30, 40);
          tft.unloadFont();
          menu(0, false, ms_settings);
          break;
        case ms_alarms:
          menu(0, false, ms_alarms);
          break;
      }
} else {
      Serial.println("Not clicked");
      curMenu = m;
      p = p + d;
      if (p < 1) p = 1;
      Serial.print("p : "); Serial.println(p);

      switch (m) {
        case ms_main:
          if (menu_main(p) > m_main_LAST) {
            p = m_main_LAST;
          } else {
            Serial.print("p + 1 : "); Serial.println(p + 1);
            strNext = items_main(menu_main(p + 1));
          }

          strThis = items_main(menu_main(p));
          if (p > 1) strPrev = items_main(menu_main(p - 1));


          break;
        case ms_settings:


          break;
        case ms_alarms:
          break;
      }

      // generate menu
      Serial.print("strPrev: "); Serial.println(strPrev);
      Serial.print("strThis: "); Serial.println(strThis);
      Serial.print("strNext: "); Serial.println(strNext);

      tft.setTextColor(con.element.FG_COLOUR, con.element.BG_COLOUR);

      tft.loadFont(F18L);
      tft.fillRect(80, 93, 210, 25, con.element.BG_COLOUR);
      tft.drawString(strPrev, 70, 95);

      tft.fillRect(80, 178, 210, 25, con.element.BG_COLOUR);
      tft.drawString(strNext, 70, 180);
      tft.unloadFont();

      tft.loadFont(F24L);
      tft.fillRect(90, 133, 210, 25, con.element.BG_COLOUR);
      tft.drawString(strThis, 90, 135);
      tft.unloadFont();
  }

  if (GRID_ON) grid(10);
}

/*******************************************************/
// Purpose   : Main Menu invoker
// Paramters : None
// Returns   : None
/*******************************************************/
void invokeMainMenu() {
  resetTFTlight();
  setButtonDefaultInMenu();
  menu(0, true, ms_main);
}

/*******************************************************/
// Purpose   : Exit Main Menu
// Paramters : None
// Returns   : None
/*******************************************************/
void exitMainMenu() {
  resetTFTlight();
  setButtonDefaultOn();
  clearScreen();
  paintRadioScreen();
}

/*******************************************************/
// Purpose   : Selector Control to Select items when in a menu
// Paramters : None
// Returns   : None
/*******************************************************/
void menuSelect() {
  menu(0);
}

/*******************************************************/
// Purpose   : Selector Control to browse items when in a menu
// Paramters : None
// Returns   : None
/*******************************************************/
void rotMenuBrowse(ESPRotary & r) {
  Serial.print("Direction: "); Serial.println (r.getDirection());
  r.getDirection() == 255 ? dir = -1 : dir = 1;
  setEvent(menuBrowse, now(), LOCAL_TIME);
}

void menuBrowse() {
  menu(dir);
}

/*******************************************************/
// Purpose   : Set button behaviour to default when
//           : displaying a menu
// Paramters : None
// Returns   : None
/*******************************************************/
void setButtonDefaultInMenu() {
  btnStandby.onPressed(disableControl);
  btnMenu.onPressed(btnExitMainMenu);
  btnMode.onPressed(disableControl);

  btnSelector.onPressed(btnMenuSelect);
  selector.setChangedHandler(rotMenuBrowse);
}

/*******************************************************/
// Purpose   : Set button behaviour to default when
//           : radio is in Off State
// Paramters : None
// Returns   : None
/*******************************************************/
void setButtonDefaultOff() {
  btnStandby.onPressed(btnStandbyTurnOn);
  btnMenu.onPressed(disableControl);
  btnMode.onPressed(callFormatSPIFFS);

  btnSnooze.onPressed(btnWeatherPressed);

  btnSelector.onPressed(disableControl);
  btnVolume.onPressed(disableControl);

  selector.setChangedHandler(disableControl);
  volume.setChangedHandler(disableControl);
}

/*******************************************************/
// Purpose   : Set button behaviour to default when
//           : radio is in On State
// Paramters : None
// Returns   : None
/*******************************************************/
void setButtonDefaultOn() {
  btnStandby.onPressed(btnStandbyTurnOff);
  btnMenu.onPressed(btnMenuPressed);
  btnMode.onPressed(btnModePressed);

  btnSnooze.onPressed(btnWeatherPressed);

  btnSelector.onPressed(btnSelectorPressed);
  btnVolume.onPressed(btnVolumePressed);

  selector.setChangedHandler(selectorChanged);
  volume.setChangedHandler(volumeChanged);
}

void disableControl() {
  // do nothing
}

void disableControl(ESPRotary & r) {
  // do nothing
}
