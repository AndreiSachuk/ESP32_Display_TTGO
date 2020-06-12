#ifndef LEDS_STAT_H
#define LEDS_STAT_H

#include <Arduino.h>
#include <SmartLeds.h>


extern SmartLed leds;



void showRgb(uint8_t, uint8_t, uint8_t);
void led(bool k);
void led_single(int k);


#endif