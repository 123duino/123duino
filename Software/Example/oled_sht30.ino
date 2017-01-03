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
#include "Adafruit_SHT31.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SHT31 sht31 = Adafruit_SHT31();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

// If using software SPI (the default case):
#define OLED_DC 9
#define OLED_CS 7 
#define OLED_CLK 6 
#define OLED_MOSI 8 
#define OLED_RESET 4 

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
/*
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
*/
void setup()   {          
  Wire.begin();
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
 
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    display.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  display.display();
  delay(700);
  // Clear the buffer.
  display.clearDisplay();

 
 
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  display.drawLine(0, 28, 128, 28, WHITE);
  display.drawLine(0, 30, 128, 30, WHITE);
    
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(3);
  display.print(t);
  display.setTextSize(2);
  display.setCursor(100,8);
  display.print("C");
  display.drawCircle(95,6,2, WHITE);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,36);
  display.setTextSize(3);
  display.print(h);
  display.setTextSize(2);
  display.setCursor(100,45);
  display.print("%");
  
 
  display.display();
  
  
  delay(400);
  display.clearDisplay();
  
}


