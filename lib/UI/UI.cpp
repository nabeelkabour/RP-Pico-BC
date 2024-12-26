#include "UI.h"
//#include <Bicycle Computer.h>

UI::Manager menuManager;

UI::Manager::Manager()
{
    menuPages[Page::NONE] = nullptr;
    menuPages[Page::spdDst] = new UI::SpdDstPage("Speed/Dist");
    menuPages[Page::main] = new UI::MainMenu("Main Menu");
    menuPages[Page::settings] = new UI::SettingsMenu("Settings");
    menuPages[Page::variable] = new UI::Integer("Volume", 0, 100, 25, nullptr);
    menuPages[Page::setContrast] = new UI::Integer("Contrast", 0, 127, 60, &contrast);
    menuPages[Page::bikeCirc] = new UI::BikeCirc("Wheel circ mm");

    // currentPage = Page::main;
    // prevPage = Page::main;
}

void UI::Manager::update()
{
    menuPages[currentPage]->update();
 
//    int32_t encVal = encoder.getCount();
//    uint8_t menuSize = menuPages[currentPage]->elementList.size(); 

//    if(encVal < 0) encoder.reset(0);
//    else if(encVal > menuSize) encoder.reset(menuSize - 1);

//    menuManager.cursor = encoder.getCount();
}

void UI::Manager::confirm()
{
    menuPages[currentPage]->confirm();   
}

void UI::Manager::back()
{
    menuPages[currentPage]->back();
}

UI::MenuPage::MenuPage(std::string _name) : pageName(_name) {}
UI::Info::Info(std::string _name) : MenuPage(_name) {}

void UI::MenuPage::update()
{
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print(pageName.c_str());  
    display.setTextColor(BLACK, WHITE);
    display.display();
}

void UI::MenuPage::confirm()
{
    display.clearDisplay();
}

void UI::MenuPage::back()
{
    display.clearDisplay();
}

UI::ListPage::ListPage(std::string _name) : MenuPage(_name) {}

void UI::ListPage::confirm()
{
    MenuPage::confirm();

    elementList[menuManager.cursor]->action();
}

void UI::ListPage::back()
{
    MenuPage::back();
    
    menuManager.currentPage = menuManager.prevPage;
    menuManager.prevPage = Page::main;

    display.clearDisplay();
}

//*************************Main Menu*********************************

UI::MainMenu::MainMenu(std::string _name) : ListPage(_name)
{
    elementList.push_back(new Elements::MenuTransfer("Settings", UI::Page::settings));
    elementList.push_back(new Elements::MenuTransfer("Variable", UI::Page::variable));
    elementList.push_back(new Elements::MenuTransfer("Main display", UI::Page::spdDst));
}

void UI::MainMenu::update()
{
    ListPage::update();
}

void UI::MainMenu::confirm()
{
    ListPage::confirm();
}

void UI::MainMenu::back()
{
    ListPage::back();
}

//**********************************************************************

void UI::Elements::MenuTransfer::action()
{
    menuManager.prevPage = menuManager.currentPage;
    menuManager.currentPage = destinationPage;
    menuManager.cursor = 0;

    display.clearDisplay();
}

UI::SettingsMenu::SettingsMenu(std::string _name) : ListPage(_name)
{
    elementList.push_back(new Elements::MenuTransfer("main menu", Page::main));
    elementList.push_back(new Elements::MenuTransfer("Wheel circ", UI::Page::bikeCirc));
    elementList.push_back(new Elements::MenuTransfer("Set contrast", UI::Page::setContrast));
}

void UI::ListPage::update()
{
    MenuPage::update();

   int32_t encVal = encoder.getCount();
   uint8_t menuSize = elementList.size(); 

   if(encVal < 0) encoder.reset(0);
   else if(encVal > (menuSize - 1)) encoder.reset(menuSize - 1);

   menuManager.cursor = encoder.getCount();

    //display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println();

    int8_t index = 0;

    for(auto& element : elementList)
    {
        if(menuManager.cursor == index) display.print(">");
        else display.print(" ");
        
        display.print(element->name.c_str());
        display.println();
        display.display();

        ++index;
    }    
}

void UI::SettingsMenu::update()
{
    ListPage::update();
}

void UI::SettingsMenu::confirm()
{
    ListPage::confirm();
}

UI::Variable::Variable(std::string _name, int16_t _min, int16_t _max, int16_t defaultVal, int16_t* _varPtr) :
    MenuPage(_name), min(_min), max(_max), defaultValue(defaultVal), variablePtr(_varPtr) {}

UI::Integer::Integer(std::string _name, int16_t _min, int16_t _max, int16_t defaultVal, int16_t* _varPtr) :
    Variable(_name, _min, _max, defaultVal, _varPtr) {}

void UI::Integer::update()
{
    MenuPage::update();

    int32_t encVal = encoder.getCount();
    if(encVal < min) encoder.reset(min);
    else if(encVal > max) encoder.reset(max);

    menuManager.cursor = encoder.getCount();

    value = menuManager.cursor;

    if(value != prevValue)
    {
        saved = false;
        prevValue = value;
    }

    if(savedValue == value) saved = true;

    if(value < min) value = min;
    else if (value > max) value = max;

    if(saved)
    {
        display.setCursor(42, 10);
        display.setTextSize(1);
        display.print("SAVED");
    }
    else
    {
        display.setCursor(42, 10);
        display.setTextSize(1);
        display.print("     ");
    }
    display.setTextSize(3);
    display.setCursor(0, 18);
    display.print("    ");
    display.setCursor(0, 18);
    display.print(value);
    display.display();
}

void UI::Integer::confirm()
{ 
    MenuPage::confirm();

    savedValue = value;
    *variablePtr = value;
    //*variablePtr = value; Store on SD card
    saved = true;
}

void UI::Integer::back()
{
    MenuPage::back();

    menuManager.currentPage = menuManager.prevPage;
    menuManager.prevPage = Page::main;

    display.clearDisplay();
}

UI::BikeCirc::BikeCirc(std::string _name) : MenuPage(_name) {}

void UI::BikeCirc::update()
{
    MenuPage::update();

   int32_t encVal = encoder.getCount();
   if(encVal < 0) encoder.reset(0);
   else if(encVal > 9) encoder.reset(9);

   menuManager.cursor = encoder.getCount();

    if(saved)
    {
        display.setCursor(42,10);
        display.setTextSize(1);
        display.print("SAVED");
    }
    else
    {
        display.setCursor(42, 10);
        display.setTextSize(1);
        display.print("     ");
    }

    display.setTextSize(3);
    display.setCursor(0, 22);

    switch(stage)
    {
        case 0:
        { 
            if(wait(prevBlinkTime, BLINK_INTERVAL))
            {
                blinkStatus = !blinkStatus;
            }

            bikeDigits[3] = menuManager.cursor;

            if(blinkStatus)
            {
                display.print(int(bikeDigits[3]));
            } 
            else
            {
                display.print(" ");
            }

            display.print(int(bikeDigits[2]));
            display.print(int(bikeDigits[1]));
            display.print(int(bikeDigits[0]));

            break;
        }

        case 1:
        {
            if(wait(prevBlinkTime, BLINK_INTERVAL))
            {
                blinkStatus = !blinkStatus;
            }

            bikeDigits[2] = menuManager.cursor;

            display.print(int(bikeDigits[3]));

            if(blinkStatus)
            {
                display.print(int(bikeDigits[2]));
            } 
            else
            {
                display.print(" ");
            }

            display.print(int(bikeDigits[1]));
            display.print(int(bikeDigits[0]));
            
            break;
        }

        case 2:
        {
            if(wait(prevBlinkTime, BLINK_INTERVAL))
            {
                blinkStatus = !blinkStatus;
            }

            display.print(int(bikeDigits[3]));
            display.print(int(bikeDigits[2]));

            bikeDigits[1] = menuManager.cursor;
            
            if(blinkStatus)
            {
                display.print(int(bikeDigits[1]));
            } 
            else
            {
                display.print(" ");
            }

            display.print(int(bikeDigits[0]));

            break;
        }

        case 3:
        {
            if(wait(prevBlinkTime, BLINK_INTERVAL))
            {
                blinkStatus = !blinkStatus;
            }

            display.print(int(bikeDigits[3]));
            display.print(int(bikeDigits[2]));

            display.print(int(bikeDigits[1]));

            bikeDigits[0] = menuManager.cursor;
            
            if(blinkStatus)
            {
                display.print(int(bikeDigits[0]));
            } 
            else
            {
                display.print(" ");
            }

            break;
        }  

        case 4:
        {
            if(wait(prevBlinkTime, BLINK_INTERVAL))
            {
                blinkStatus = !blinkStatus;
            }


            if(blinkStatus)
            {
                display.print(int(bikeDigits[3]));
                display.print(int(bikeDigits[2]));
                display.print(int(bikeDigits[1]));
                display.print(int(bikeDigits[0]));
            } 
            else
            {
                display.print("    ");
            }

            break;
        }    

        case 5:
        {
            display.print(int(bikeDigits[3]));
            display.print(int(bikeDigits[2]));
            display.print(int(bikeDigits[1]));
            display.print(int(bikeDigits[0]));

            break;
        }  
    }

    bikeCirc = bikeDigits[3]*1000 + bikeDigits[2]*100 + bikeDigits[1]*10
                + bikeDigits[0];

    saved = bikeCirc == savedValue;
}

void UI::BikeCirc::confirm()
{
    switch(stage)
    {
        case 0:
        {
            //bikeDigits[3] = menuManager.cursor;
            ++stage;
            break;
        }
        
        case 1:
        {
            //bikeDigits[2] = menuManager.cursor;
            ++stage;
            break;
        }

        case 2:
        {
            //bikeDigits[1] = menuManager.cursor;
            ++stage;
            break;
        }
        
        case 3:
        {
            //bikeDigits[0] = menuManager.cursor;
            ++stage;
            break;
        }

        case 4:
        {
            //convert digits to int
            bikeCirc = bikeDigits[3]*1000 + bikeDigits[2]*100 + bikeDigits[1]*10
                        + bikeDigits[0];

            savedValue = bikeCirc;
            saved = true;

            // if(bikeCirc != savedValue)
            // {
            //     saved = false;
            //     savedValue = bikeCirc;
            // }
            //Store wheel circ SD card

            ++stage;
            break;
        }
    }
}

void UI::BikeCirc::back()
{
    display.clearDisplay();

    if(stage > 0)
    {
        --stage;
         saved = false;
    }
    else
    {
        menuManager.currentPage = menuManager.prevPage;
        menuManager.prevPage = UI::Page::main;
    }
}

void UI::SpdDstPage::update()
{
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print("KM");
    display.setCursor(70, 8);
    display.print("/H");
    display.setCursor(70, 24);
    display.print("KM");
    display.display();

    display.fillRect(0, 0, 70, 24, 0);
    display.setTextSize(3);

    uint8_t decimal_places_spd = 1;

    display.setCursor(0, 0);
    if (bcData.speed < 10.0F)
    {
        display.print(" ");
    }
    else if (bcData.speed < 100.0F)
    {
        decimal_places_spd = 1;
    }
    else if (bcData.speed < 1000.0F)
    {
        decimal_places_spd = 0;
        display.print(" ");
    }

    display.print(bcData.speed, decimal_places_spd);
    display.display();

    
    display.fillRect(0, 24, 70, 24, 0);
    display.setTextSize(3);

    double distance_km = double(bcData.distance) / 1000000.0;

    uint8_t decimal_places_dst = 0;

    display.setCursor(0, 24);
    if (distance_km < 10.0)
    {
        decimal_places_dst = 2;
        // display.print("   ");
    }
    else if (distance_km < 100.0)
    {
        decimal_places_dst = 1;
        // display.print("  ");
    }
    else if (distance_km < 1000.0)
    {
        // decimal_places = 0;
        display.print(" ");
    }

    // display.print(distance_km * 1000, 2);

    display.print(distance_km, decimal_places_dst);
    display.display();
}

void UI::SpdDstPage::confirm()
{
    MenuPage::confirm();
    
    menuManager.currentPage = UI::Page::main;
    menuManager.prevPage = UI::Page::spdDst;
}

void UI::SpdDstPage::back()
{
    MenuPage::back();

    menuManager.currentPage = UI::Page::main;
    menuManager.prevPage = UI::Page::bikeCirc;
}

UI::SpdDstPage::SpdDstPage(std::string _name) : Info(_name) {}