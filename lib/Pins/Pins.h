#ifndef PINS_H
#define PINS_H

#define REED_SWITCH_PIN 15
//#define BLE_PIN 4

#define SDCS 22
#define SD_LED 6

#define SPI_CLK 18
#define SPIMOSI 19

#define DISP_DC 20
#define DISP_CS 17 
#define DISP_RST 21

#define ENC_A 11
#define ENC_B 12
#define ENC_BTN 13

#define BACK_BTN 14

namespace Pins
{
    void setup();
}

#endif