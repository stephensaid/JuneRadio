#ifndef weather_h
#define weather_h

#include <Arduino.h>
#include <WiFi.h>
#include <defaults.h>
#include <ezTime.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include <rjConfig.h>
#include <tft.h>

extern rjConfig con;

void getCurrentWeather();
String getWeatherIcon(String);

#endif // weather_h
