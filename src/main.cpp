#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"
#include "bmp.h"
#include <SoftwareSerial.h>
#include "leds_stat.h"
#include <analogWrite.h>

#define RX 22                        
#define TX 17 
#define LED_PIN 26

char data[7]; 

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4   // Display backlight control pin
#define BUTTON_1        35
#define BUTTON_2        0

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

char buff[512];
int vref = 1100;
int btnCick = false;
byte k;

uint8_t Red, Green, Blue, Single;

void displayTest(){
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    if (TFT_BL > 0) { 
        pinMode(TFT_BL, OUTPUT); 
        digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); 
    }

    tft.setSwapBytes(true);

    tft.setRotation(0);
    tft.fillScreen(TFT_BLUE);
    delay(300);
    tft.setTextColor(0x0000);
    tft.drawString("Light", 65, 1);
    tft.drawString("Control", 65, 40);
    tft.drawString("System", 65, 80);
    tft.drawString("With ", 65, 120);
    tft.drawString("Automatic", 60, 160);
    tft.drawString("Calibration", 40, 200);
    delay(3000);

}

void displayStart(){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(0xF800);
    tft.drawString("RED: ", 1, 1);
    tft.setTextColor(0x07E0);
    tft.drawString("GREEN: ", 1, 40);
    tft.setTextColor(0x001F);
    tft.drawString("BLUE: ", 1, 80);
    tft.setTextColor(0xFFFF);
    tft.drawString("SINGLE: ", 1, 120);
    tft.setTextDatum(TL_DATUM);
}

void Radio(){
  if (Serial2.available() && Serial2.read() == 0xAA && Serial2.read() == 0xBB){
    Serial2.readStringUntil('\n').toCharArray(data, sizeof(data));
    Red = data[0];
    Green = data[1];
    Blue = data[2];
    Single = data[3];}
}

void RGBprint_display(){
    tft.setTextColor(0xF800,0x0000);
    tft.drawString(String(Red)+"   ", 50, 1);
    tft.setTextColor(0x07E0,0x0000);
    tft.drawString(String(Green)+"   ", 75, 30);
    tft.setTextColor(0x001F,0x0000);
    tft.drawString(String(Blue)+"   ", 60, 70);
    tft.setTextColor(0xFFFF,0x0000);
    tft.drawString(String(Single)+"   ", 85, 110);
 
}
     

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600,SERIAL_8N1,RX,TX);
    Serial.println("Start");
    displayTest();
    displayStart();
}


void loop()
{ Radio();
  RGBprint_display();
  showRgb(Red,Green,Blue);
  analogWrite(LED_PIN, Single);
}