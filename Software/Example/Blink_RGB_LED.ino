/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

const int R_LED_Pin= 5;      // the number of the RED LED pin
const int G_LED_Pin= 10;     // the number of the GREEN LED pin
const int B_LED_Pin= 11;     // the number of the BLUE LED pin

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(R_LED_Pin, OUTPUT);
  pinMode(G_LED_Pin, OUTPUT);
  pinMode(B_LED_Pin, OUTPUT);
  
  digitalWrite(R_LED_Pin, HIGH);
  digitalWrite(G_LED_Pin, HIGH);
  digitalWrite(B_LED_Pin, HIGH);
 
}

// the loop function runs over and over again forever
void loop() {
  

  digitalWrite(R_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(500);                      // wait for 500ms
  digitalWrite(R_LED_Pin, HIGH);   // turn the LED off (HIGH is the voltage level)
  delay(300);                      // wait for 300ms
  
  digitalWrite(G_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(500);                      // wait for 500ms
  digitalWrite(G_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(300);                      // wait for 300ms
 
  digitalWrite(B_LED_Pin, LOW);    // turn the LED on by making the voltage LOW
  delay(500);                      // wait for 500ms
  digitalWrite(B_LED_Pin,HIGH);    // turn the LED off (HIGH is the voltage level)
  delay(300);                      // wait for 300ms
 
 
}
