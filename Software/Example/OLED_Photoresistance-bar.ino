/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_DC 9 
#define OLED_CS 7 
#define OLED_CLK 6 
#define OLED_MOSI 8 
#define OLED_RESET 4 

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

#define bar_start_x 1
#define bar_start_y 10
#define bar_height 30
#define bar_width 3
#define bar_space 1
#define Max 950

void setup()   {          

  delay(900);              // wait for a second

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(800);

  
}


void loop() {
  display.setTextSize(1);

  // read the analog in value:
  sensorValue = analogRead(analogInPin); //正常(亮)0 ~ 1024(暗) 
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 30, 0); //權範圍感測
  
  if (sensorValue < Max) sensorValue = Max;  //縮小感測範圍,讓感測較為明顯變化
  outputValue = map(sensorValue, Max, 1023,55, 0);

  // change the analog out value:
  DrawBar(outputValue);
  display.setTextColor(WHITE);
  display.setCursor(1,30);

  display.println("");

  display.println("");
  display.print("Photo:");
  display.print(sensorValue);
  display.print("/");
  display.println(outputValue);
 
  display.display();
  delay(40);
  display.clearDisplay();
  
}

void DrawBar(int num){
  for (int x=0;x<num;x++)
       display.fillRect(bar_start_x +(bar_width*x+bar_space*x),bar_start_y,bar_width,bar_height,1);
  //display.display();
};



