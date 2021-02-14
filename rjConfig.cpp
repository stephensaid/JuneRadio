// https://arduinojson.org/v6/example/config/

#include "rjConfig.h"

#define _conFilename "/config.txt"

rjConfig::rjConfig() {
  // _conFilename = "/config.txt";
}

void rjConfig::loadConfig() {
  loadConfiguration(_conFilename, element);
}

void rjConfig::saveConfig() {
  saveConfiguration(_conFilename, element);
}

void rjConfig::printConfig() {
  printFile(_conFilename);
}

void rjConfig::resetConfig() {
  // resets configuration file to default
  resetConfiguration(_conFilename, element);
}

//////////////////// PRIVATE FUCNTIONS ////////////////////////

// Loads the configuration from a file
void rjConfig::loadConfiguration(const char *filename, Config &config) {
  // Open file for reading
  File file = SPIFFS.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(14) + 230;
  DynamicJsonDocument doc(capacity);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("************************************************"));
    Serial.println(F("Failed to read file, using default configuration"));
    Serial.println(F("************************************************"));
  }

  // Copy values from the JsonDocument to the Config
  config.WIFI_SSID        = doc["WIFI_SSID"] | "";
  config.WIFI_PASSWORD    = doc["WIFI_PASSWORD"] | "";
  config.WEATHER_TIMEOUT  = doc["WEATHER_TIMEOUT"] | 300;
  config.SCREEN_TIMEOUT   = doc["SCREEN_TIMEOUT"] | 10;
  config.TIME_FORMAT      = doc["TIME_FORMAT"] | 1;                 // 1= 12 hour clock; 2 = 24 hour clock
  config.BG_COLOUR        = doc["BG_COLOUR"] | TFT_BLACK;
  config.FG_COLOUR        = doc["FG_COLOUR"] | TFT_WHITE;
  config.HIGHLIGHT_COLOUR = doc["HIGHLIGHT_COLOUR"] | TFT_GREEN;
  config.ALARM_1_HOUR     = doc["ALARM_1_HOUR"] | 25;
  config.ALARM_1_MINUTE   = doc["ALARM_1_MINUTE"] | 0;
  config.ALARM_2_HOUR     = doc["ALARM_2_HOUR"] | 25;
  config.ALARM_2_MINUTE   = doc["ALARM_2_MINUTE"] | 0;
  config.ALARM_3_HOUR     = doc["ALARM_3_HOUR"] | 25;
  config.ALARM_3_MINUTE   = doc["ALARM_3_MINUTE"] | 0;

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  return;
}

// Saves the configuration to a file
void rjConfig::saveConfiguration(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(14);
  DynamicJsonDocument doc(capacity);

  // Set the values in the document
  doc["WIFI_SSID"]        = config.WIFI_SSID;
  doc["WIFI_PASSWORD"]    = config.WIFI_PASSWORD;
  doc["WEATHER_TIMEOUT"]  = config.WEATHER_TIMEOUT;
  doc["SCREEN_TIMEOUT"]   = config.SCREEN_TIMEOUT;
  doc["TIME_FORMAT"]      = config.TIME_FORMAT;           // 1= 12 hour clock; 2 = 24 hour clock
  doc["BG_COLOUR"]        = config.BG_COLOUR;
  doc["FG_COLOUR"]        = config.FG_COLOUR;
  doc["HIGHLIGHT_COLOUR"] = config.HIGHLIGHT_COLOUR;
  doc["ALARM_1_HOUR"]     = config.ALARM_1_HOUR;
  doc["ALARM_1_MINUTE"]   = config.ALARM_1_MINUTE;
  doc["ALARM_2_HOUR"]     = config.ALARM_2_HOUR;
  doc["ALARM_2_MINUTE"]   = config.ALARM_2_MINUTE;
  doc["ALARM_3_HOUR"]     = config.ALARM_3_HOUR;
  doc["ALARM_3_MINUTE"]   = config.ALARM_3_MINUTE;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void rjConfig::printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
  return;
}

void rjConfig::resetConfiguration(const char *filename, Config &config) {
  config.WIFI_SSID        = "";
  config.WIFI_PASSWORD    = "";
  config.WEATHER_TIMEOUT  = 5 * 60;
  config.SCREEN_TIMEOUT   = 10;
  config.TIME_FORMAT      = 1;                //  1= 12 hour clock; 2 = 24 hour clock
  config.BG_COLOUR        = TFT_BLACK;
  config.FG_COLOUR        = TFT_WHITE;
  config.HIGHLIGHT_COLOUR = TFT_GREEN;
  config.ALARM_1_HOUR     = 25;
  config.ALARM_1_MINUTE   = 0;
  config.ALARM_2_HOUR     = 25;
  config.ALARM_2_MINUTE   = 0;
  config.ALARM_3_HOUR     = 25;
  config.ALARM_3_MINUTE   = 0;

  saveConfig();
}
