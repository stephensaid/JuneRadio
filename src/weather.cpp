#include "weather.h"

OpenWeatherMapCurrent weatherClient;           // initiate the client
WiFiClient wifiClient;                  // Initiate WiFi client
OpenWeatherMapCurrentData weatherData;  // stores weather data in weatherData
time_t lastWeatherUpdate;

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

  weatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  weatherClient.setMetric(IS_METRIC);
  weatherClient.updateCurrentById(&weatherData, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  yield();    // allow program to continue running while we wait for response

  lastWeatherUpdate = now();
  Serial.print("getCurrentWeather()::Last weather forecast updated: ");
  Serial.println( lastWeatherUpdateDateTime() );

  Serial.println("------------------------------------");

  // "lon": 8.54, float lon;
  Serial.printf("lon: %f\n", weatherData.lon);
  // "lat": 47.37 float lat;
  Serial.printf("lat: %f\n", weatherData.lat);
  // "id": 521, weatherId weatherId;
  Serial.printf("weatherId: %d\n", weatherData.weatherId);
  // "main": "Rain", String main;
  Serial.printf("main: %s\n", weatherData.main.c_str());
  // "description": "shower rain", String description;
  Serial.printf("description: %s\n", weatherData.description.c_str());
  // "icon": "09d" String icon; String iconMeteoCon;
  Serial.printf("icon: %s\n", weatherData.icon.c_str());
  Serial.printf("iconMeteoCon: %s\n", weatherData.iconMeteoCon.c_str());
  // "temp": 290.56, float temp;
  Serial.printf("temp: %f\n", weatherData.temp);
  // "pressure": 1013, uint16_t pressure;
  Serial.printf("pressure: %d\n", weatherData.pressure);
  // "humidity": 87, uint8_t humidity;
  Serial.printf("humidity: %d\n", weatherData.humidity);
  // "temp_min": 289.15, float tempMin;
  Serial.printf("tempMin: %f\n", weatherData.tempMin);
  // "temp_max": 292.15 float tempMax;
  Serial.printf("tempMax: %f\n", weatherData.tempMax);
  // "wind": {"speed": 1.5}, float windSpeed;
  Serial.printf("windSpeed: %f\n", weatherData.windSpeed);
  // "wind": {"deg": 1.5}, float windDeg;
  Serial.printf("windDeg: %f\n", weatherData.windDeg);
  // "clouds": {"all": 90}, uint8_t clouds;
  Serial.printf("clouds: %d\n", weatherData.clouds);
  // "dt": 1527015000, uint64_t observationTime;
  time_t time = weatherData.observationTime;
  Serial.printf("observationTime: %d, full date: %s", weatherData.observationTime, ctime(&time));
  // "country": "CH", String country;
  Serial.printf("country: %s\n", weatherData.country.c_str());
  // "sunrise": 1526960448, uint32_t sunrise;
  time = weatherData.sunrise;
  Serial.printf("sunrise: %d, full date: %s", weatherData.sunrise, ctime(&time));
  // "sunset": 1527015901 uint32_t sunset;
  time = weatherData.sunset;
  Serial.printf("sunset: %d, full date: %s", weatherData.sunset, ctime(&time));

  // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", weatherData.cityName.c_str());
  Serial.println();
  Serial.println("---------------------------------------------------/\n");
}

/*******************************************************/
// Function  : Private function
// Purpose   :
// Paramters : None
// Returns   : Returns string with time of last updated
//           : Example 03:28
/*******************************************************/
String lastWeatherUpdateDateTime() {
  return dateTime(lastWeatherUpdate, "H:i");
}
