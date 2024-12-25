#include <BikeTime.h>
#include <Pins.h>
#include <Display.h>
//#include <Input.h>
#include "Bicycle Computer.h"

//uint16_t count = 0;
// float distance = 0;
//uint32_t distance = 0;
//float speed = 0;
//uint16_t tire_circ = 2110;
// float tire_circ_km = 0.00211F;
// uint32_t poll_rate = 0;

BcData bcData;

time_ms prev_update_time = 0;

void BicycleComputer::pollHallsensor()
{
    static bool prev_reedswitch_value = HIGH;
    bool reedswitch_value;

    reedswitch_value = digitalRead(REED_SWITCH_PIN);

    if (reedswitch_value != prev_reedswitch_value)
    {
        if (reedswitch_value == HIGH)
        {
            ++bcData.count;
        }

        prev_reedswitch_value = reedswitch_value;
    }

   //count = inputData.encoderPos;
}

void BicycleComputer::measureSpeedDistance()
{
    time_ms now = millis();
    time_ms delta_time = now - prev_update_time;

    if (delta_time >= BC_MEASURE_INTERVAL)
    {
        if (bcData.count)
        {
            float revolutions = float(bcData.count) / float(BC_NUM_MAGNETS);
            //distance += revolutions * tire_circ_km;
            bcData.distance += revolutions * bcData.tire_circ;
            bcData.speed = revolutions * float(bcData.tire_circ) / float(delta_time) * 3.6F;

            if(bcData.speed > 9999.0F)
            {
                bcData.speed = 9999.0F;
            }

            bcData.count = 0;
            //encoder.step = 0;
        }
        else
        {
            bcData.speed = 0.0F;
        }

        //Display::showSpeed();
        //Display::showDistance();
        prev_update_time = now;
    }
}

void BicycleComputer::loop()
{
    pollHallsensor();
    measureSpeedDistance();
}