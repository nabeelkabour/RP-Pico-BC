#include <Arduino.h>
#include "BikeTime.h"

unsigned int operator""_sec(unsigned long long seconds)
{
    return seconds * 1000; // seconds to milliseconds
}

double operator""_sec(long double seconds)
{
    return seconds * 1000.0;
}

unsigned int operator""_min(unsigned long long mins)
{
    return mins * 60_sec;
}

double operator""_min(long double mins)
{
    return mins * 60.0_sec;
}

bool wait(unsigned long &prev_time, unsigned long const interval)
{
    time_ms now = millis();

    if (now - prev_time >= interval)
    {
        prev_time = now;
        return true;
    }
    return false;
}