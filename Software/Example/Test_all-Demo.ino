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
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Eeprom24C128_256.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;



// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


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

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int R_LED_Pin= 5;      // the number of the RED LED pin
const int G_LED_Pin= 10;     // the number of the GREEN LED pin
const int B_LED_Pin= 11;     // the number of the BLUE LED pin

int speakerPin = 12;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

const int buttonPin_R = 0;    // the number of the pushbutton pin
const int buttonPin_L = 1;     // the number of the pushbutton pin
/**************************************************************************//**
 * \def EEPROM_ADDRESS
 * \brief Address of EEPROM memory on TWI bus.
 ******************************************************************************/
#define EEPROM_ADDRESS  0x50

/******************************************************************************
 * Private variable definitions.
 ******************************************************************************/

static Eeprom24C128_256 eeprom(EEPROM_ADDRESS);
  //  int address = 0 ;

void setup()   {          
  Wire.begin();
    
      eeprom.initialize();


    
  pinMode(speakerPin, OUTPUT);
  
  pinMode(R_LED_Pin, OUTPUT);
  pinMode(G_LED_Pin, OUTPUT);
  pinMode(B_LED_Pin, OUTPUT);
  
  digitalWrite(R_LED_Pin, HIGH);
  digitalWrite(G_LED_Pin, HIGH);
  digitalWrite(B_LED_Pin, HIGH);
  
  pinMode(buttonPin_R, INPUT_PULLUP);
  pinMode(buttonPin_L, INPUT_PULLUP);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
 
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(3500);
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
  
    accelgyro.initialize();
   
    // verify connection
    display.println("Testing device connections..."); 
  
    display.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


  display.display();
  delay(1000);
  // Clear the buffer.
  display.clearDisplay();

 
 
}


void loop() {

  Test_MPU6050();
  delay(100);
  display.clearDisplay();
  display.display();
  
  
  SHT30_Test();
  delay(100);
  display.clearDisplay();
  display.display();
   
  EEPROM_test();
  delay(100);
  display.clearDisplay();
  display.display();
 
  Blink_RGB_LED();
  delay(100);
  display.clearDisplay();
  display.display();
  
  Buzzer();
  delay(100);
  display.clearDisplay();
  display.display();
   

   
  Photoresistanc();
  delay(100);
  display.clearDisplay();
  display.display();
   
  Key_test();
  delay(100);
  display.clearDisplay(); 
  display.display();
}

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

#define bar_start_x 1
#define bar_start_y 10
#define bar_height 30
#define bar_width 3
#define bar_space 1
#define Max 950

void Photoresistanc()
{
 int test_count=0;
do
{
  test_count++;
 display.setTextSize(1);

  // read the analog in value:
  sensorValue = analogRead(analogInPin); //正常(亮)0 ~ 1024(暗) 
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 30, 0); //全範圍感測
  
  if (sensorValue < Max) sensorValue = Max;  //縮小感測範圍,讓感測較為明顯變化
  outputValue = map(sensorValue, Max, 1023, 55, 0);

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
  
  
} while (test_count < 120);
}


void DrawBar(int num){
  for (int x=0;x<num;x++)
       display.fillRect(bar_start_x +(bar_width*x+bar_space*x),bar_start_y,bar_width,bar_height,1);
  //display.display();
};


void Buzzer()
{
  //程式碼參考來自
//http://thats-worth.blogspot.tw/2014/05/arduino-controlling-piezo.html


// 依照簡譜的順序，填入代表的音符，空白代表休止符
char notes[] = "ccggaagffeeddc ";
// 決定每個音階的拍子，注意這邊用 unsigned long 所以拍子只能是正整數
unsigned long beats[] = {1,1,1,1,1,1,2,1,1,1,1,1,1,2,4}; 
// 利用 sizeof()，算出總共要多少音符
int length = sizeof(notes); 
// 決定一拍多長，這邊一拍 300 ms
int tempo = 300;


  // 利用 for 來播放我們設定的歌曲，一個音一個音撥放
  for (int i = 0; i < length; i++) {
 // 如果是空白的話，不撥放音樂
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
  // 呼叫 palyNote() 這個 function，將音符轉換成訊號讓蜂鳴器發聲
      playNote(speakerPin,notes[i], beats[i] * tempo);
    } 
    // 每個音符之間的間隔，這邊設定的長短會有連音 or 段音的效果
    delay(tempo/10); 
  }
}
void playNote(int OutputPin, char note, unsigned long duration) {
   // 音符字元與對應的頻率由兩個矩陣表示
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 261, 294, 330, 349, 392, 440, 494, 523 };
  // 播放音符對應的頻率
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      tone(OutputPin,tones[i], duration);
  //下方的 delay() 及 noTone ()，測試過後一定要有這兩行，整體的撥放出來的東西才不會亂掉，可能是因為 Arduino 送出tone () 頻率後會馬上接著執行下個指令，不會等聲音播完，導致撥出的聲音混合而亂掉
      delay(duration);
      noTone(OutputPin);
    }
  }
}


void Blink_RGB_LED()
{
 int test_count=0;
do
{
  test_count++;
  digitalWrite(R_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(200);                      // wait for 200ms
  digitalWrite(R_LED_Pin, HIGH);   // turn the LED off (HIGH is the voltage level)
  delay(200);                      // wait for 200ms
  
  digitalWrite(G_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(200);                      // wait for 200ms
  digitalWrite(G_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(200);                      // wait for 200ms
 
  digitalWrite(B_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(200);                      // wait for 200ms
  digitalWrite(B_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(200);                      // wait for 200ms 
  
} while (test_count < 5);
test_count=0;
do
{
  test_count++;
  digitalWrite(R_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(90);                      // wait for 200ms
  digitalWrite(R_LED_Pin, HIGH);   // turn the LED off (HIGH is the voltage level)
  delay(90);                      // wait for 200ms
  
  digitalWrite(G_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(90);                      // wait for 200ms
  digitalWrite(G_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(90);                      // wait for 200ms
 
  digitalWrite(B_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(90);                      // wait for 200ms
  digitalWrite(B_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(90);                      // wait for 200ms 
  
} while (test_count < 15);


}


void Key_test()
{
  // variables will change:
int buttonState_R = 0;         // variable for reading the pushbutton status
int buttonState_L = 0;         // variable for reading the pushbutton status


 long int test_count=0;
 
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,0);
 display.print("Press the right or left key");  
 display.display();
  
do
{
  test_count++;
    // read the state of the pushbutton value:
  buttonState_R = digitalRead(buttonPin_R);
  buttonState_L = digitalRead(buttonPin_L);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
 if (buttonState_R == HIGH) {
    // turn LED on:
    digitalWrite(R_LED_Pin, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(R_LED_Pin, LOW);
  }


  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState_L == HIGH) {
    // turn LED on:
    digitalWrite(G_LED_Pin, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(G_LED_Pin, LOW);
  }

  
  
  
  
} while (test_count < 700000);
}

    
void Test_MPU6050()
{
 int test_count=0;
 bool blinkState = false;
do
{
  test_count++;
  
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(0,0);
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    
    display.setTextSize(1);
    display.print("ax=");display.println(ax); 
    display.print("ay=");display.println(ay); 
    display.print("az=");display.println(az); 
    display.print("gx=");display.println(gx); 
    display.print("gy=");display.println(gy); 
    display.print("gz=");display.println(gz); 
    
        display.display();
      delay(100);
      display.clearDisplay();
  

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(G_LED_Pin, blinkState); 

  
} while (test_count < 90);
} 
    

void SHT30_Test()
{
 int test_count=0;
do
{
  test_count++;
  
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
  
  
  delay(200);
  display.clearDisplay();
  
} while (test_count < 10);


}


 void Alarm()
 {
   for (int x = 0;x<10 ; x++){
     
     tone(speakerPin,550,220); 
     digitalWrite(R_LED_Pin,HIGH);
     delay(130); 
     
     noTone(speakerPin); 
     tone(speakerPin,1350,100); 
     digitalWrite(R_LED_Pin,LOW);
     
     delay(100); 
     noTone(speakerPin); 
     digitalWrite(R_LED_Pin,HIGH);
   }
 }
 
 void EEPROM_test()
 {
       const word address = 0;
    display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(0,0);
  
 
    display.setTextSize(1);
    
      // Write a byte at address 0 in EEPROM memory.
    display.println("Write EEPROM ..");
    eeprom.writeByte(address, 0x42);  // Write "B"
    
    // Write cycle time (tWR). See EEPROM memory datasheet for more details.
    delay(20);
    
    // Read a byte at address 0 in EEPROM memory.
    display.println("Read EEPROM ..");
    byte data = eeprom.readByte(address);
   
    display.println("---------------------");
    // Print read byte.
    display.print("Address =");display.println(address);
    display.print("Read byte = 0x");display.println(data);
   
    if (data==0x42)
       display.println("PASS");
    else
    {
       display.println("FAIL");
       Alarm();
    }
    display.display();
    delay(4000);
    display.clearDisplay();
  

 }
 
