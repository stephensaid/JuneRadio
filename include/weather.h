#ifndef weather_h
#define weather_h

#include <Arduino.h>
#include <WiFi.h>
#include <defaults.h>
#include <ezTime.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include <rjConfig.h>

extern rjConfig con;

void getCurrentWeather();
String getWeatherIcon(String);
// String lastWeatherUpdateDateTime();

#endif // weather_h
