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

// If using software SPI (the default case):
#define OLED_DC 9 
#define OLED_CS 7 
#define OLED_CLK 6
#define OLED_MOSI 8 
#define OLED_RESET 4 
const int piezoPin = 12; 

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SHT31 sht31 = Adafruit_SHT31();


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

#define start_x 16
#define end_x 127

#define T_start_y 28
#define T_end_y 0
#define H_start_y 60
#define H_end_y 31
#define Buttom_time_out 1500 //
#define Temp_High_Limit 40
#define Temp_Low_Limit  4

#define Humidity_High_Limit 98
#define Humidity_Low_Limit  10

bool Counter_start = 0;
bool Temp_Alarm_ON = 0;
bool Humi_Alarm_ON = 1;
const long interval = 100;           // (Millis seconds)

unsigned long Counter = 0 ;
unsigned long previousMillis = 0;       
unsigned long currentMillis =0 ;
unsigned long time_counter =0 ;

float Alarm_Temp_High =31.00; 
float Alarm_Temp_Low =22.00; 

float Alarm_Humidity_High =80.00; 
float Alarm_Humidity_Low =40.00; 

const int R_LED_Pin= 5;      // the number of the LED pin
const int G_LED_Pin= 10;      // the number of the LED pin
const int B_LED_Pin= 11;      // the number of the LED pin

const int buttonPin1 = 0;    // the number of the pushbutton pin
const int buttonPin2 = 1;     // the number of the pushbutton pin

// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status


int current_x = 30 ;
int previous_T_y = 0 ;
int current_T_y = 0 ;
int previous_H_y = 0 ;
int current_H_y = 0 ;

float current_t = 0;
float current_h = 0;


void setup()   {          

  Wire.begin();
  pinMode(piezoPin,OUTPUT); 
  
    // initialize the LED pin as an output:
  pinMode(R_LED_Pin, OUTPUT);
  pinMode(G_LED_Pin, OUTPUT);
  pinMode(B_LED_Pin, OUTPUT);
  
  digitalWrite(R_LED_Pin,HIGH);
  digitalWrite(G_LED_Pin,HIGH);
  digitalWrite(B_LED_Pin,HIGH);
  
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
  delay(2100);

  // Clear the buffer.
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
  
  
  MainScreen();

  Show_current();
  //display.clearDisplay();
  
     delay(10);
 
  
}


void loop() {
  
  current_t = sht31.readTemperature();
  current_h = sht31.readHumidity();

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
 
 //   float t = sht31.readTemperature();
//    float h = sht31.readHumidity();
    //MainScreen();
    Show_current();
       
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    
    if (buttonState2 == LOW) {
      delay(100);
      Menu_ON_OFF(); 
      MainScreen();    
    }
    
    
    //Alarm check 確認是否觸發警報
    if (Temp_Alarm_ON){
        if (current_t>=Alarm_Temp_High) Alarm();
        if (current_t<=Alarm_Temp_Low) Alarm();
    }
    
     if (Humi_Alarm_ON){
        if (current_h>=Alarm_Humidity_High) Alarm();
        if (current_h<=Alarm_Humidity_Low) Alarm();
    }
    

    
    
    /*
    current_T_y=map(t,10,38,T_start_y,T_end_y);
    current_H_y=map(h,10,99,H_start_y,H_end_y);
       
    display.drawLine(current_x,previous_T_y,current_x+1,current_T_y,1); 
    display.drawLine(current_x,previous_H_y,current_x+1,current_H_y,1); 

    previous_T_y = current_T_y; 
    previous_H_y = current_H_y; 
    current_x++; 
    display.display();
    */
   /* 
    if(current_x>end_x) {
        current_x = start_x;
        display.clearDisplay();
        DrawLine();
      }
    */

  }
  
 
   
  
  
  
  
  
  
  
  
/*
  display.setTextSize(1);

  // read the analog in value:
  sensorValue = analogRead(analogInPin); //正常(亮)0 ~ 1024(暗) 
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 30, 0); //權範圍感測
  
  if (sensorValue < Max) sensorValue = Max;  //縮小感測範圍,讓感測較為明顯變化
  outputValue = map(sensorValue, Max, 1023, 30, 0);

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
  */
  
}


 
 void Alarm()
 {
   for (int x = 0;x<10 ; x++){
     
     tone(piezoPin,650,230); 
     digitalWrite(R_LED_Pin,HIGH);
     delay(230); 
     
     noTone(piezoPin); 
     tone(piezoPin,1550,100); 
     digitalWrite(R_LED_Pin,LOW);
     
     delay(100); 
     noTone(piezoPin); 
     digitalWrite(R_LED_Pin,HIGH);
   }
 }
 
void Show_current()
{
  //float current_t = sht31.readTemperature();
  //float current_h = sht31.readHumidity();

 

  display.fillRect(102,0,25,10,1);  //目前溫度的白框
  display.setTextSize(1);
  display.setTextColor(0);
  display.setCursor(103,1);
  display.println(current_t);
  
  display.fillRect(102,32,25,10,1);  //目前溫度的白框
  display.setCursor(103,33);
  display.println(current_h);
  display.display();
  
  delay(10);
  display.fillRect(102,0,25,10,0);
  display.fillRect(102,32,25,10,0);
  
  //display.display();
   
}


//bool Temp_Alarm_ON 1;
//bool Humi_Alar_ON 1;
void Show_ON_OFF_Select_Menu()
{
   display.clearDisplay();
   
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.print("Temperature Alarm");
  
  if (Temp_Alarm_ON){
    display.fillRect(7,12,40,17,1);
    display.setTextColor(0);
    display.setTextSize(2);
    display.setCursor(15,13);
    display.print("ON");
   
    display.setTextColor(1); 
    display.setCursor(70,13);
    display.print("OFF");  
 
    
   }else{
    display.fillRect(63,12,46,17,1);
    display.setTextColor(0);
    display.setTextSize(2);
    display.setTextColor(0); 
    display.setCursor(70,13);
    display.print("OFF");
  
    display.setTextColor(1); 
    display.setCursor(15,13);
    display.print("ON"); 

    
   }
  
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(0,33);
  display.println("Humidity Alarm"); 

  if (Humi_Alarm_ON){
     display.fillRect(7,12+33,40,17,1);
     display.setTextColor(0); 
     display.setTextSize(2);
     display.setCursor(15,13+33);
     display.print("ON");
     
     display.setTextColor(1); 
     display.setCursor(70,13+33);
     display.print("OFF");

  }else{
     display.fillRect(63,12+33,46,17,1);
     display.setTextColor(0); 
     display.setTextSize(2);
     display.setCursor(70,13+33);
     display.print("OFF");
    
     display.setTextColor(1);
     display.setCursor(15,13+33);
     display.print("ON");
    
  }
  
  display.display();
  
}

void Menu_ON_OFF()
{
 
  Show_ON_OFF_Select_Menu();
  int btn_sel = 0;
  //---------------------------------------------------------------
  
  int time_out_counter = 0;
  unsigned long previous_Time_out_Millis = 0 ;
  //---------------------------------------------------------------
  do{
    
    ///Time out counter
    unsigned long current_Time_out_Millis = millis();

    if (current_Time_out_Millis - previous_Time_out_Millis >= 1000) {
        previous_Time_out_Millis = current_Time_out_Millis;
        time_out_counter++;       
    }
    if (time_out_counter >= 3) break; ///break out Do While
    
    ///End Time out counter
    
    
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
     //debug
     //display.fillRect(1,13,12,12,0); //clear 
     //display.setTextColor(1); 
     //display.setTextSize(1);
     //display.setCursor(1,15);
     //display.print(btn_sel);
     //////////////////////////////////
     
     display.display();
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
   if (buttonState2 == LOW) {
      // turn LED on:
      digitalWrite(R_LED_Pin, HIGH);
      btn_sel++;
      delay(200);    //避免開關彈跳
      if (btn_sel>=4) btn_sel=0;
      
          //清除前一個選項框
      display.drawRect(7-2,12-2,40+4,17+4,0);
      display.drawRect(63-2,12-2,46+4,17+4,0);
      display.drawRect(7-2,12+33-2,40+4,17+4,0);
      display.drawRect(63-2,12+33-2,46+4,17+4,0);
      display.display();
      
      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
  
    }
    
    //顯示選擇項目框框
    switch (btn_sel) {
    case 0:
      //do something when var equals 0
      display.drawRect(7-2,12-2,40+4,17+4,1);
      
      break;
    case 1:
      //do something when var equals 1
      display.drawRect(63-2,12-2,46+4,17+4,1);
      break;
    case 2:
      //do something when var equals 2
      display.drawRect(7-2,12+33-2,40+4,17+4,1);
      break;
    case 3:
      //do something when var equals 3
      display.drawRect(63-2,12+33-2,46+4,17+4,1);
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }


    display.display();
    

    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState1 == LOW) {
      switch (btn_sel) {
          case 0:
              Menu_Temperature_ON_SEL();
              Temp_Alarm_ON = 1;
              Show_ON_OFF_Select_Menu();
              break;
          case 1:
              //Menu_Temperature_ON_SEL();
              Temp_Alarm_ON = 0;
              Show_ON_OFF_Select_Menu();
              break;
          case 2:
              Menu_Humidity_ON_SEL();
              Humi_Alarm_ON = 1;
              Show_ON_OFF_Select_Menu();
              break;
          case 3:
              //Menu_Temperature_ON_SEL();
              Humi_Alarm_ON = 0;
              Show_ON_OFF_Select_Menu();
              break;
      }    
            
      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
    
    }
    
    
    }while(1);
  //---------------------------------------------------------------
  
}

void Menu_Temperature_ON_SEL()
{
  int set_value=0;
  display.clearDisplay();
   
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.print("Temperature Alarm Set");
  

    display.setTextColor(1);
    display.setTextSize(2);
    display.setCursor(11,13);
    display.print("High");
   
   
    display.setCursor(11,13+33);
    display.print("Low");
     
    display.display();
  
  int btn_sel = 0;
  
  int time_out_counter = 0;
  unsigned long previous_Time_out_Millis = 0 ;
  //---------------------------------------------------------------
  do{
    
    ///Time out counter
    unsigned long current_Time_out_Millis = millis();

    
    if (current_Time_out_Millis - previous_Time_out_Millis >= 1000) {
        previous_Time_out_Millis = current_Time_out_Millis;
        time_out_counter++;       
    }
    if (time_out_counter >= 3) break; ///break out Do While
    
    ///End Time out counter
    
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
     //debug
     //display.fillRect(1,32,12,11,0); //clear 
     //display.setTextColor(1); 
     //display.setTextSize(1);
     //display.setCursor(1,34);
     //display.print(set_value);
     //////////////////////////////////
     //顯示高低溫值
     display.setTextColor(1); 
     display.setTextSize(2);
     display.setCursor(65,13);
     display.print(Alarm_Temp_High); 
     
     display.setCursor(65,46);
     display.print(Alarm_Temp_Low); 
     
     
     display.display();
   // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState2 == LOW) {        //選擇設High or Low 的值
      btn_sel++;
      delay(200);    //避免開關彈跳
      if (btn_sel>=2) btn_sel=0;
      
      //清除前一個選項
      display.drawRect(5,10,55,21,0);
      display.drawRect(5,43,55,21,0);
      display.display();

      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
  
    }
    
    
    //顯示選擇項目框框
    switch (btn_sel) {
    case 0:
      //do something when var equals 0
      display.drawRect(5,10,55,21,1);
      
      break;
    case 1:
      //do something when var equals 2
      display.drawRect(5,43,55,21,1);
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
    
    }


    display.display();
    

 
    
   // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState1 == LOW) {
      if (btn_sel){  //設定高溫值
        Alarm_Temp_Low+=0.2;
        delay(10);    //避免開關彈跳
      }else{        //設定低溫值
        Alarm_Temp_High+=0.2;
        delay(10);    //避免開關彈跳
     
      }
      
       //低溫不會超過高溫
      if (Alarm_Temp_Low>Alarm_Temp_High) Alarm_Temp_Low = Temp_Low_Limit;
      if (Alarm_Temp_High>Temp_High_Limit) Alarm_Temp_High = Alarm_Temp_Low + 5;
     
      //清除前一個數值
      display.fillRect(64,10,60,54,0);
      //display.display();

      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
      
    }
   //End if 
   
    
    }while(1);
  //---------------------------------------------------------------
  
}



void Menu_Humidity_ON_SEL()
{
  int set_value=0;
  display.clearDisplay();
   
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.print("Humidity Alarm Set");
  

    display.setTextColor(1);
    display.setTextSize(2);
    display.setCursor(11,13);
    display.print("High");
   
   
    display.setCursor(11,13+33);
    display.print("Low");
     
    display.display();
  
  int btn_sel = 0;
  
  int time_out_counter = 0;
  unsigned long previous_Time_out_Millis = 0 ;
  //---------------------------------------------------------------
  do{
    
    ///Time out counter
    unsigned long current_Time_out_Millis = millis();

    
    if (current_Time_out_Millis - previous_Time_out_Millis >= 1000) {
        previous_Time_out_Millis = current_Time_out_Millis;
        time_out_counter++;       
    }
    if (time_out_counter >= 3) break; ///break out Do While
    
    ///End Time out counter
    
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
     //debug
     //display.fillRect(1,32,12,11,0); //clear 
     //display.setTextColor(1); 
     //display.setTextSize(1);
     //display.setCursor(1,34);
     //display.print(set_value);
     //////////////////////////////////
     //顯示高低濕度
     display.setTextColor(1); 
     display.setTextSize(2);
     display.setCursor(65,13);
     display.print(Alarm_Humidity_High); 
     
     display.setCursor(65,46);
     display.print(Alarm_Humidity_Low); 
     
     
     display.display();
   // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState2 == LOW) {        //選擇設High or Low 的值
      btn_sel++;
      delay(200);    //避免開關彈跳
      if (btn_sel>=2) btn_sel=0;
      
      //清除前一個選項
      display.drawRect(5,10,55,21,0);
      display.drawRect(5,43,55,21,0);
      display.display();

      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
  
    }
    
    
    //顯示選擇項目框框
    switch (btn_sel) {
    case 0:
      //do something when var equals 0
      display.drawRect(5,10,55,21,1);
      
      break;
    case 1:
      //do something when var equals 2
      display.drawRect(5,43,55,21,1);
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
    
    }


    display.display();
    

 
    
   // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState1 == LOW) {
      if (btn_sel){  //設定高溫值
        Alarm_Humidity_Low+=0.5;
        delay(10);    //避免開關彈跳
      }else{        //設定低溫值
        Alarm_Humidity_High+=0.5;
        delay(10);    //避免開關彈跳
     
      }
      
       //低溫不會超過高溫
      if (Alarm_Humidity_Low>Alarm_Humidity_High) Alarm_Humidity_Low = Humidity_Low_Limit;
      if (Alarm_Humidity_High>Humidity_High_Limit) Alarm_Humidity_High = Alarm_Humidity_Low + 5;
     
      //清除前一個數值
      display.fillRect(64,10,60,54,0);
      //display.display();

      previous_Time_out_Millis=  millis(); //有按按鍵清除time out counter
      time_out_counter =0;
      
    }
   //End if 
   
    
    }while(1);
  //---------------------------------------------------------------
  
}


void MainScreen()
{
  // Clear the buffer.
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.print("Temperature Alarm");
  
  display.setCursor(58,12);
  display.print("High");
  display.setCursor(58,22);
  display.print("Low");
  
  display.fillRect(7,12,40,17,1);
  display.setTextColor(0);
  display.setTextSize(2);
  if (Temp_Alarm_ON){
      display.setCursor(15,13);
      display.print("ON");
  }else{
      display.setCursor(10,13);
      display.print("OFF");
  }
  
 


  
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(0,33);
  display.println("Humidity Alarm"); 

  display.setCursor(58,12+33);
  display.print("High");
  display.setCursor(58,22+33);
  display.print("Low");
  
  display.fillRect(7,12+33,40,17,1);
  display.setTextColor(0); 
  display.setTextSize(2);
  if (Humi_Alarm_ON){
      display.setCursor(15,13+33);
      display.print("ON");
  }else{
      display.setCursor(10,13+33);
      display.print("OFF");
  }
  


 
  display.setTextColor(1);
  display.setTextSize(1);
  display.setCursor(90,12);  //High temp
  display.print(Alarm_Temp_High);
  display.print("C");
  
  
  display.setCursor(90,22);  //Low temp
  display.print(Alarm_Temp_Low);
  display.print("C");
   
  display.setTextSize(1);
  display.setCursor(90,12+33);  //High temp
  display.print(Alarm_Humidity_High);
  display.print("%");
  
  display.setCursor(90,22+33);  //Low temp
  display.print(Alarm_Humidity_Low);
  display.print("%");
    
  
  display.display();

  
}


