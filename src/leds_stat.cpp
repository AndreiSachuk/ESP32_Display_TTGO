#include "leds_stat.h"

const int LED_COUNT = 300;
const int DATA_PIN = 12;
const int CHANNEL = 0;
SmartLed leds( LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer );

void showRgb(uint8_t r, uint8_t g, uint8_t b) {
	for (int i=0; i<LED_COUNT; i++)
    leds[ i ] = Rgb{ r, g, b };
    leds.show();
}

void led(bool k){
	if (k==true) digitalWrite(2, HIGH);
	else digitalWrite(2,LOW);
}
