#include <SPI.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC 9 
#define OLED_CS 7 
#define OLED_CLK 6 
#define OLED_MOSI 8 
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//右按鈕開始/暫停
//左邊按鈕歸零
const int buttonPin1 = 0;    // the number of the pushbutton pin
const int buttonPin2 = 1;     // the number of the pushbutton pin
// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status

bool Counter_start = 0;
const long interval = 10;           // (micro seconds)

unsigned long Counter = 0 ;
unsigned long previousMicros = 0 ;
unsigned long currentMicros =0 ;
  
void setup() {


  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2500);
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(3,10);
  display.println("Counter");
  
  display.setTextSize(3);
  display.setTextColor(1);
  display.setCursor(3,40);
  display.print("0");
  display.display();

}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  

   
   
  if ( buttonState2 == LOW){  //左鍵為歸零
     Counter = 0;
     delay(20); //防開關彈跳延遲
  
     display.setTextSize(2);
     display.setTextColor(1);
     display.setCursor(3,10);
     display.println("Counter");
  
     display.setTextSize(3);
     display.setTextColor(1);
     display.setCursor(3,40);
     display.print("0");
     display.display();
     currentMicros = micros(); //計數器清為0
     Counter_start = 0 ; //設為暫停
  }
  
  if ( buttonState1 == LOW){  //右鍵為暫停與開始
      Counter_start = !Counter_start;
      delay(250); //防開關彈跳延遲 
  }
            

    

  if (Counter_start==1)  
  {

     display.setTextSize(2);
     display.setTextColor(1);
     display.setCursor(3,10);
     display.println("Counter");
  
     display.setTextSize(3);
     display.setTextColor(1);
     display.setCursor(3,40);
     
     currentMicros =micros();
     if (currentMicros - previousMicros >= interval) {
        // save the last time you blinked the LED
        previousMicros = currentMicros;
        Counter++;
      }
     
     display.print(Counter);
     display.display();
    // delay(1);
     display.clearDisplay();
  }else  // 暫停
  {
   currentMicros = micros();
  }

  
}



