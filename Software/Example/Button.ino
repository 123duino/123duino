/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:

const int R_LED_Pin= 5;      // the number of the RED LED pin
const int G_LED_Pin= 10;     // the number of the GREEN LED pin
const int B_LED_Pin= 11;     // the number of the BLUE LED pin

const int buttonPin_R = 0;    // the number of the pushbutton pin
const int buttonPin_L = 1;     // the number of the pushbutton pin

// variables will change:
int buttonState_R = 0;         // variable for reading the pushbutton status
int buttonState_L = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(R_LED_Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin_R, INPUT_PULLUP);
  
    // initialize the LED pin as an output:
  pinMode(G_LED_Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin_L, INPUT_PULLUP);
  
  digitalWrite(R_LED_Pin, HIGH);
  digitalWrite(G_LED_Pin, HIGH);
}

void loop() {
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


}
