#include "weather.h"

OW_Weather ow; // Weather forecast library instance

OW_current  *currentWeather = new OW_current;
OW_hourly   *hourly         = new OW_hourly;
OW_daily    *daily          = new OW_daily;

extern bool redraw;

/***************************************************************/
// Function  : Public  function
// Purpose   : Get weather update from OpenWeatherMap using API
// Paramters : None
// Returns   : None - sets arrays with weather data
/***************************************************************/
void getCurrentWeather() {
  // weatherUpdateHandle = setEvent(getCurrentWeather, now() + weatherUpdateFrequency );
  setEvent(getCurrentWeather, now() + weatherUpdateFrequency );
  if ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("getCurrentWeather():: Weather update failed because Wi-Fi is not connected!");
    setEvent(reConnectWiFi, now() + 1);
    return;
  }
  if (DEBUG_INFO) Serial.println("getCurrentWeather():: Requesting weather information from OpenWeatherMap.org...");

  String units;
  IS_METRIC == true ? units = "metric" : units = "imperial";

  bool parsed = ow.getForecast(currentWeather, nullptr, nullptr, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LATITUDE, OPEN_WEATHER_MAP_LONGITUDE, units, OPEN_WEATHER_MAP_LANGUAGE);

  if (parsed) {
    redraw = true;
    if (DEBUG_INFO)Serial.println("getCurrentWeather():: Data points received");
  } else {
    if (DEBUG_DEBUG) Serial.println("getCurrentWeather():: Failed to get data points");
  }
}

/***************************************************************/
// Function  : Public  function
// Purpose   : Get weather update from OpenWeatherMap using API
// Paramters : String with icon received from OpenWeatherMap
// Returns   : String with jpg file
/***************************************************************/
String getWeatherIcon(String icon) {
  String bg, icon_return;
  con.element.BG_COLOUR == TFT_DKGRAY ? bg = "blk" : bg = "wht";

  if ( icon.startsWith("01") || icon.startsWith("02") || icon.startsWith("10") ) {
    icon_return = icon;
    if (DEBUG_INFO) Serial.printf("1. icon_return: %s\n", icon_return.c_str());
  } else {
    icon_return = icon.substring(0, 2);
    if (DEBUG_INFO) Serial.printf("2. icon_return: %s\n", icon_return.c_str());
  }

  if (DEBUG_INFO) {
    Serial.print("JPG: ");
    Serial.println("/weather/" + icon_return + "-" + bg +".jpg");
  }
  return "/weather/" + icon_return + "-" + bg +".jpg";
}
