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

//автотаймер через какое время, что выполнять EXEC_TIMER_SET(Tafter,Tcode)
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)
#define EXEC_TIMER_SET_NM(Tname,Tafter,Tcode) \
{                                             \
  static uint32_t Tname = millis()+Tafter;    \
  if(Tname < millis()){                       \
    Tname = millis()+Tafter;                  \
    {Tcode}                                   \
  }                                           \
}
#define EXEC_TIMER_SET(Tafter,Tcode) EXEC_TIMER_SET_NM(COMBINE(__FUNCTION__,__LINE__),Tafter,Tcode)


//Конвертнуть из строки в бул
bool toBool(String s);

//Конвертим #****** в цвета
#define getColor(r,g,b) (r|g<<8|b<<16)
#define getColorR(v) (v&0xFF)
#define getColorG(v) ((v>>8)&0xFF)
#define getColorB(v) ((v>>16)&0xFF)

uint32_t hexColorToInt(String s);

//Вывести в сериал переменную LOG_DEBUG_VARIABLE(VAR_NAME)
#define LOG_DEBUG_VARIABLE(VAR_NAME) {Serial.println(String(#VAR_NAME)+":"+String(VAR_NAME));}

//ретурнуть последнюю измененную переменную из этих двух, указав их тип
#define GET_LAST_CHANGED(TYPE_NAME, IN_X1, IN_X2)\
([](TYPE_NAME inX1, TYPE_NAME inX2) -> TYPE_NAME {\
  static TYPE_NAME x1 = inX1;\
  static TYPE_NAME x2 = inX2;\
  static TYPE_NAME def = inX1;\
\
  if(x1 != inX1){\
    def = inX1;\
    x1 = inX1;\
    return inX1;\
  }\
  if(x2 != inX2){\
    def = inX2;\
    x2 = inX2;\
    return inX2;\
  }\
\
  return def;\
})(IN_X1, IN_X2);

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
#define ADC_EN          14  //ADC_EN is the ADC detection enable port
#define ADC_PIN         34
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

    if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
        pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
        digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }

    tft.setSwapBytes(true);

    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
    delay(300);
    tft.fillScreen(TFT_BLUE);
    delay(300);
    tft.fillScreen(TFT_GREEN);
    delay(300);
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
 if (Serial2.available() && Serial2.read() == 0xAA && Serial2.read() == 0xBB)
  Serial2.readStringUntil('\n').toCharArray(data, sizeof(data));
  Red = data[0];
  Green = data[1];
  Blue = data[2];
  Single = data[3];
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
    tft.fillRect(0, 200, 135, 40, 0xF1FF);
    
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