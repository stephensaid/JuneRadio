#ifndef rjConfig_h
#define rjConfig_h

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <defaults.h>

#define _conFilename "/config.txt"

class rjConfig {
  public:
    void loadConfig();
    void saveConfig();
    void printConfig();
    void resetConfig();
    struct Config {
      String WIFI_SSID;
      String WIFI_PASSWORD;
      int WEATHER_TIMEOUT;
      int SCREEN_TIMEOUT;
      int TIME_FORMAT;                // 1 = 12 hour clock; 2 = 24 hour clock
      unsigned int BG_COLOUR;
      unsigned int FG_COLOUR;
      unsigned int HIGHLIGHT_COLOUR;
      int ALARM_1_HOUR;
      int ALARM_1_MINUTE;
      int ALARM_2_HOUR;
      int ALARM_2_MINUTE;
      int ALARM_3_HOUR;
      int ALARM_3_MINUTE;
    };
    Config element;
  private:
    void loadConfiguration(const char *filename, Config &config);
    void saveConfiguration(const char *filename, const Config &config);
    void printFile(const char *filename);
    void resetConfiguration(const char *filename, Config &config);
    // String _conFilename;
    // File _configFile;
};

#endif //rjConfig_h
