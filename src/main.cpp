#include <Arduino.h>
#include "PicoEncoder.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <UI.h>

//Adafruit_PCD8544 display = Adafruit_PCD8544(SPI_CLK, SPIMOSI, DISP_DC, DISP_CS, DISP_RST);
PicoEncoder encoder;

bool oldConfirmBtnState = true;
bool oldBackBtnState = true;
bool checkConfirmButton();
bool checkBackButton();

void setup()
{
  Display::setup();

  encoder.begin(ENC_A);

  //Serial.begin(115200);
}

void loop()
{
  BicycleComputer::loop();
  
  encoder.update();
  
  menuManager.cursor = encoder.step / 4;

  if(checkConfirmButton()) menuManager.confirm();
  if(checkBackButton()) menuManager.back();

  menuManager.update();

  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.setTextSize(5);
  // display.print(int32_t(encoder.step) / 4); 
  // display.display();
  // Serial.println(int(encoder.step) / 4);
}

bool checkConfirmButton()
{
    bool isPressed = false;

    bool btnState = digitalRead(ENC_BTN);
    if (btnState != oldConfirmBtnState) 
    {
        oldConfirmBtnState = btnState;
        if (btnState == LOW)  
        {
            isPressed = true;
        }
        delay(20);
    }

    return isPressed;
}

bool checkBackButton()
{
    bool isPressed = false;

    bool btnState = digitalRead(BACK_BTN);
    if (btnState != oldBackBtnState) 
    {
        oldConfirmBtnState = btnState;
        if (btnState == LOW)  
        {
            isPressed = true;
        }
        delay(20);
    }

    return isPressed;
}