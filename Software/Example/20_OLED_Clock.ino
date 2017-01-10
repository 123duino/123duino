
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// If using software SPI (the default case):
#define OLED_DC 9 //8    //11
#define OLED_CS 7 //10   //12
#define OLED_CLK 6 //13  //10
#define OLED_MOSI 8 //11 //9
#define OLED_RESET 4 //11//112 //13  V2 11->4

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#define LED_PIN 11
bool blinkState = false;
int Hour = 12;
int Minute = 30;
int Second = 30;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;     

// constants won't change :
const long interval = 1000;           // 1 second
int step_speed =1000;
int step_speed_counter=0;

const int buttonPin1 = 0;    // the number of the pushbutton pin
const int buttonPin2 = 1;     // the number of the pushbutton pin
// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
long button_counter=0;

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
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  previousMillis = millis();
  button_counter = millis();
}
void loop(){    

  unsigned long currentMillis = millis();

  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if ( buttonState1 == LOW){
      if (currentMillis - button_counter >=step_speed) {
        step_speed_counter++;
        button_counter = currentMillis;
        Minute++;
        Second =0;
        if (Minute >=60) {Minute=0;Hour++;}
        if (Hour>12) {Hour=1;}
        
        if (step_speed_counter<3) {step_speed = 900;}
        else if (step_speed_counter >=3 && step_speed_counter <16) {step_speed = 300;}
        else {step_speed = 40;}
          
        // display.setCursor(10,3);display.print(step_speed_counter); //for debug
        // display.setCursor(10,10);display.print(step_speed); //for debug
      }
  }

  if ( buttonState2 == LOW){
      if (currentMillis - button_counter >=step_speed) {
        step_speed_counter++;
        button_counter = currentMillis;
        Minute--;
        Second =0;
        if (Minute <0) {Minute=59;Hour--;}
        if (Hour<=0) {Hour=12;}
        
        if (step_speed_counter<3) {step_speed = 900;}
        else if (step_speed_counter >=3 && step_speed_counter <16) {step_speed = 300;}
        else {step_speed = 80;}
          
         //display.setCursor(10,3);display.print(step_speed_counter); for debug
         //display.setCursor(10,10);display.print(step_speed); for debug
      }
  }
  if (buttonState2 ==HIGH && buttonState1 == HIGH) {
    step_speed = 1000;
    step_speed_counter =0;
    step_speed = 10;
  }

  
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Second++;
      if (Second >=60) {Second=0;Minute++;}
      if (Minute >=60) {Minute=0;Hour++;}
      if (Hour>12) {Hour=1;Hour++;}
      
      
   }

  display.fillRect(4,40,Second*2,5,1); 
  display.setTextSize(2);
 
 

  display.setCursor(10,15);
  if (Hour<10) {display.print(" ");}
  display.print(Hour);
  display.print(":");
  if (Minute<10) {display.print("0");}
  display.print(Minute);
  display.print(":");
  if (Second<10) {display.print("0");}
  display.print(Second);
 
  display.setTextSize(1);
  display.setCursor(5,50);
  display.print("123Duino Clock Demo");
 
 
  display.display();
  display.clearDisplay();
   



}

