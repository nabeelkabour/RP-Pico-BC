#include <Arduino.h>
#include "Pins.h"

void Pins::setup()
{
    pinMode(REED_SWITCH_PIN, INPUT);
    pinMode(BACK_BTN, INPUT);
    pinMode(ENC_BTN, INPUT);
    // pinMode(SD_LED, OUTPUT);
    //pinMode(SDCS, OUTPUT);
   // pinMode(BLE_PIN, OUTPUT);
}