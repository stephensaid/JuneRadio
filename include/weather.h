#ifndef weather_h
#define weather_h

#include <Arduino.h>
#include <WiFi.h>
#include <defaults.h>
#include <ezTime.h>
#include <OpenWeather.h>

// uint8_t weatherUpdateHandle;

OpenWeatherMapCurrent client;         // initiate the client
WiFiClient wifiClient;                // Initiate WiFi client

class weather {
  public:
    void getCurrentWeather();
};

#endif // weather_h
