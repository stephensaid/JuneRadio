#include "weather.h"

OpenWeatherMapCurrent client;         // initiate the client
WiFiClient wifiClient;                // Initiate WiFi client

/*******************************************************/
// Function  : Public  function
// Purpose   : Get weather update from OpenWeatherMap using API
// Paramters : None
// Returns   : None - sets arrays with weather data
/*******************************************************/
void getCurrentWeather() {
  weatherUpdateHandle = setEvent(getCurrentWeather, now() + weatherUpdateFrequency );
  if ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("getCurrentWeather()::Weather update failed because Wi-Fi is not connected!");
    return;
  }

  Serial.println("getCurrentWeather()::Requesting weather information from OpenWeatherMap.org...");

  OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  yield();    // allow program to continue running while we wait for response


  // to change below code
  //   if ( parsed ) {
  //     lastWeatherUpdate = now();
  //     // show last updated timestamp of weather forecast
  //     Serial.print("getCurrentWeather()::Last weather forecast updated: ");
  //     Serial.println( lastWeatherUpdateDateTime() );
  //   } else Serial.println("getCurrentWeather()::Weather failed to parse!");
}

/*******************************************************/
// Function  : Private function
// Purpose   :
// Paramters : None
// Returns   : Returns string with time of last updated
//           : Example 03:28
/*******************************************************/
// String lastWeatherUpdateDateTime() {
//   return dateTime(lastWeatherUpdate, "H:i");
// }
