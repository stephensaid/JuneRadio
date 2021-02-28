#ifndef helpers_h
#define helpers_h

#include <Arduino.h>
#include <rjConfig.h>
#include <TFT_eSPI.h>
#include <defaults.h>
#include <ezTime.h>
#include <EasyButton.h>
#include <ESPRotary.h>
#include <WiFi.h>

void drawKnob();
void drawArrow( int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t);
int fillArc(int, int, int, int, int, int, int, unsigned int);
int getDecimal(float, int);
String degToCompass (float);
void grid (int);
void resetAlarms();
void setOffAlarm();
void stopAlarm();
void snoozeAlarm();
void selector_interrupt();
void volume_interrupt();
void btnSelectorInterruptISR();
void btnVolumeInterruptISR();
void btnModeInterruptISR();
void btnStandbyInterruptISR();
void btnSnoozeInterruptISR();
void btnMenuInterruptISR();
void listAllFiles();
void deleteEvents();
void wait(unsigned long);
void print_wakeup_reason();
bool tft_output(int16_t, int16_t, uint16_t, uint16_t, uint16_t*);
void Start_WiFi();
void reConnectWiFi();
void getInternetTime();
void formatSPIFFS();


#endif // helpers_h
