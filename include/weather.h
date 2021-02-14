#ifndef weather_h
#define weather_h

#include "Arduino.h"
#include <ezTime.h>
#include <WiFi.h>
#include <defaults.h>
#include <OpenWeather.h>

OpenWeatherMapCurrent client;         // initiate the client
WiFiClient wifiClient;                // Initiate WiFi client

time_t lastWeatherUpdate;
uint8_t weatherUpdateHandle;

// weather
void getCurrentWeather();
String lastWeatherUpdateDateTime();

#endif // weather_h
