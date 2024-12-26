#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Pins.h>

extern Adafruit_PCD8544 display;

extern int16_t contrast;

namespace Display
{
    void setup();
    // void showSpeed();
    // void showDistance();
}

#endif