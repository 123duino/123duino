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

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define bar_start_x 15
#define bar_start_y 5
#define bar_height 17
#define bar_width 110
#define bar_space 20
#define value_max 150
#define Add_step 20 


int R_value =0;
int G_value = 0;
int B_value = 0;

const int R_LED_Pin= 5;      // the number of the LED pin
const int G_LED_Pin= 10;      // the number of the LED pin
const int B_LED_Pin= 11;      // the number of the LED pin

const int buttonPin1 = 0;    // the number of the pushbutton pin
const int buttonPin2 = 1;     // the number of the pushbutton pin
// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int Select_RGB = 0 ;


int adj = 0;
void setup() {

  // initialize the LED pin as an output:
  pinMode(R_LED_Pin, OUTPUT);
  pinMode(G_LED_Pin, OUTPUT);
  pinMode(B_LED_Pin, OUTPUT);
  
  analogWrite(R_LED_Pin, 255);
  analogWrite(G_LED_Pin, 255);
  analogWrite(B_LED_Pin, 255);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  

  //digitalWrite(R_LED_Pin, LOW);
  //analogWrite(R_LED_Pin, brightness);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(3500);
  display.clearDisplay();


}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  
  if ( buttonState2 == LOW){
     Select_RGB++;
     if (Select_RGB > 2) Select_RGB = 0;
     
     delay(10);
  };
  
  switch (Select_RGB) {
    case 0:
    {
      display.fillRect(bar_start_x-15,bar_start_y,13,18,1);
      display.drawChar(1,8,'R',0,1,2);
      display.drawChar(1,8+bar_space*1,'G',1,0,2);
      display.drawChar(1,8+bar_space*2,'B',1,0,2);
          
       if ( buttonState1 == LOW){
           R_value=R_value+Add_step;
           delay(10); 
      };
      break;
    }
    
    case 1:
    {
      display.fillRect(bar_start_x-15,bar_start_y+bar_space*1,13,18,1);
      display.drawChar(1,8,'R',1,0,2);
      display.drawChar(1,8+bar_space*1,'G',0,1,2);
      display.drawChar(1,8+bar_space*2,'B',1,0,2);
      
      if ( buttonState1 == LOW){
           G_value=G_value+Add_step;
           delay(10); 
      };
      break;
    }    
    case 2:
    {
      display.fillRect(bar_start_x-15,bar_start_y+bar_space*2,13,18,1);
      display.drawChar(1,8,'R',1,0,2);
      display.drawChar(1,8+bar_space*1,'G',1,0,2);
      display.drawChar(1,8+bar_space*2,'B',0,1,2);

      if ( buttonState1 == LOW){
           B_value=B_value+Add_step;
           delay(10); 
      };
      break;
    }  
    default:
      break;
  }
  


  //  R LED 
  // **************************************************************************
  if (( R_value >bar_width) && (R_value<(bar_width + Add_step)) ) R_value =bar_width;
  if ( R_value >= (bar_width+Add_step) ) R_value =0;

  display.fillRect(bar_start_x,bar_start_y,R_value,bar_height,1);
  display.drawRect(bar_start_x,bar_start_y,bar_width,bar_height,1);

  adj = map(R_value,0,bar_width,255,value_max);
  analogWrite(R_LED_Pin,adj);
  // **************************************************************************
 
  //  G LED 
  // **************************************************************************
  if (( G_value >bar_width) && (G_value<(bar_width + Add_step)) ) G_value =bar_width;
  if ( G_value >= (bar_width+Add_step) ) G_value =0;
   
  display.fillRect(bar_start_x,bar_start_y+bar_space*1,G_value,bar_height,1);
  display.drawRect(bar_start_x,bar_start_y+bar_space*1,bar_width,bar_height,1);

  adj = map(G_value,0,bar_width,255,value_max);
  analogWrite(G_LED_Pin,adj);
  // **************************************************************************
 
  //  B LED 
  // **************************************************************************
  if (( B_value >bar_width) && (B_value<(bar_width + Add_step)) ) B_value =bar_width;
  if ( B_value >= (bar_width+Add_step) ) B_value =0;
  
  display.fillRect(bar_start_x,bar_start_y+bar_space*2,B_value,bar_height,1);
  display.drawRect(bar_start_x,bar_start_y+bar_space*2,bar_width,bar_height,1);

  adj = map(B_value,0,bar_width,255,value_max);
  analogWrite(B_LED_Pin,adj);
  // **************************************************************************
 
 
  display.display();
  delay(10);    
  display.clearDisplay();
  
}



