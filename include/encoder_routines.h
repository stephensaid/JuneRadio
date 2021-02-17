#ifndef encoder_routines_h
#define encoder_routines_h

#include <Arduino.h>
#include <tft.h>
#include <ESPRotary.h>
#include <ezTime.h>
#include <menus.h>

void btnVolumePressed();
void volumeChanged(ESPRotary& r);
void btnSelectorPressed();
void btnMenuSelect();
void selectorChanged(ESPRotary& r);

#endif // encoder_routines_h
