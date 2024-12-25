#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <Display.h>
#include <BikeTime.h>
#include <Bicycle Computer.h>
#include <vector>

namespace UI
{
    enum Page : int8_t
    {
        NONE,
        spdDst,
        main,
        settings,
        variable,
        bikeCirc
    };

    class MenuPage;

    class Manager
    {
    public:
        //current menu index
        Page currentPage = Page::main;
        Page prevPage = Page::NONE;
        int8_t cursor = 0;

        //Menu array
        MenuPage* menuPages[sizeof(Page)];

        Manager();
        void update();
        void confirm();
        void back();
    };
 
    namespace Elements
    {
        class Element
        {
        public:
            std::string name = "";

            Element(std::string label) : name(label) {}
            virtual void action() = 0;
        };

        class MenuTransfer : public Element
        {
        public:
            Page destinationPage = Page::NONE;

            MenuTransfer(std::string label, Page destPage) : Element(label), destinationPage(destPage) {};
    
            //Transfer to new page
            virtual void action() override;
        };
    }

    class MenuPage
    {
    public:
        std::string pageName = "";

        //Menu elements
        //std::list<Elements::Element*> elementList;  
        std::vector<Elements::Element*> elementList;  

        MenuPage(std::string _name);
        virtual void update();
        virtual void confirm() = 0;
        virtual void back() = 0;
    };

    class ListPage : public MenuPage
    { 
    public:
        std::vector<Elements::Element*> elementList;
        ListPage(std::string _name);
        virtual void update() override;
        virtual void confirm() override;
        virtual void back() override;
    };

    class Info : public MenuPage
    {
    public:
        Info(std::string _name);
        virtual void update() = 0;
    };

    class SpdDstPage : public Info
    {
    public:

        SpdDstPage(std::string _name);
        void update() override;
        void confirm() override;
        void back() override;
    };

    // Base Variable
    class Variable : public MenuPage
    {
    public:
        int16_t* variablePtr = nullptr;
        int16_t min, max, value;
        int16_t prevValue = value;
        int16_t savedValue = value;
        bool saved = false;

        Variable(std::string _name, int16_t _min, int16_t _max, int16_t defaultVal, int16_t* _varPtr);
        virtual void confirm() = 0;
        virtual void back() = 0;
    };

    class Integer : public Variable
    {
    public:
        Integer(std::string _name, int16_t _min, int16_t _max, int16_t defaultVal, int16_t* _varPtr);
        virtual void update() override;
        virtual void confirm() override;
        virtual void back() override;
    };

    //Slider


    class BikeCirc : public MenuPage
    {
        uint8_t stage = 0;
        const uint16_t defaultBikeCirc = 2110;
        uint8_t bikeDigits[4] = { 0, 1, 1, 2 };
        time_ms prevBlinkTime = 0;
        bool blinkStatus = true;
        #define BLINK_INTERVAL 500
        bool saved = false;
        uint16_t savedValue = 9999;

    public:
        uint16_t bikeCirc = defaultBikeCirc;

        BikeCirc(std::string _name);
        void update() override;
        void confirm() override;
        void back() override;
    };
    
    class MainMenu : public ListPage
    {
    public:
        MainMenu(std::string _name);
        void update() override;
        void confirm() override;
        void back() override;
    };

    class SettingsMenu : public ListPage
    {
    public:
        SettingsMenu(std::string _name);
        void update() override;
        void confirm() override;
    };
}

extern UI::Manager menuManager;

#endif