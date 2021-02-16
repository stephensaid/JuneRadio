#ifndef weather_h
#define weather_h

#include <Arduino.h>
#include <WiFi.h>
#include <defaults.h>
#include <ezTime.h>
#include <OpenWeather.h>

void getCurrentWeather();
String lastWeatherUpdateDateTime();

#endif // weather_h
