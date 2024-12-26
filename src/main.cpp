#include <Arduino.h>
//#include "pio_encoder.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <UI.h>
#include <Input.h>

//Adafruit_PCD8544 display = Adafruit_PCD8544(SPI_CLK, SPIMOSI, DISP_DC, DISP_CS, DISP_RST);
// extern PioEncoder encoder;
//PioEncoder encoder(ENC_A);

bool oldConfirmBtnState = true;
bool oldBackBtnState = true;
bool checkConfirmButton();
bool checkBackButton();

void setup()
{
  Display::setup();

  encoder.begin();
  encoder.setMode(COUNT_1X);
  //Serial.begin(115200);
}

void loop()
{
  BicycleComputer::loop();
  
  //int32_t encVal = encoder.getCount();
 
//   if(encVal < 0) encoder.reset(0);
//   else if(encVal > 2) encoder.reset(2);

  //menuManager.cursor =  encoder.getCount();// / 4;

  if(checkConfirmButton()) menuManager.confirm();
  if(checkBackButton()) menuManager.back();

  menuManager.update();

  display.setContrast(contrast);
  display.display();
  
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
        delay(300); // Adjust this when using complete project
    }

    return isPressed;
}