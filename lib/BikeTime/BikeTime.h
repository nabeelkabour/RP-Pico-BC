#ifndef BIKETIME_H
#define BIKETIME_H

#define BC_MEASURE_INTERVAL 1500

#define ENC_INTERVAL 10
#define BTN_INTERVAL 250

unsigned int operator""_sec(unsigned long long);
double operator""_sec(long double);
unsigned int operator""_min(unsigned long long);
double operator""_min(long double);

using time_ms = unsigned long;

bool wait(unsigned long&, unsigned long const);

#endif