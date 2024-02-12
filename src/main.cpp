#include <Arduino.h>
#include <Romi32U4.h>
#include "Timer.h"
#include <servo32u4.h>
Romi32U4ButtonB buttonB;

/*
The FS90R continuous rotation servo converts 
standard RC servo position pulses into continuous
rotation speed. The default rest point is 1.5 ms, but
this can be adjusted by using a small screwdriver
to turn the middle-point adjustment potentiometer.
Pulse widths above the rest point result in counterclockwise 
rotation, with speed increasing as the pulse width increases;
pulse widths below the rest point result in clockwise rotation, 
with speed increasing as the pulse width decreases.
*/

int servoPin = 5;
int linearPotPin = 18;
int servoStop = 1490;  
int servoJawDown = 1300; 
int servoJawUp = 1700;   
int printDelay = 500;
int linearPotVoltageADC = 500;
int jawOpenPotVoltageADC = 550;
int jawClosedPotVoltageADC = 940;
int threshold = 3;

Servo32U4 jawServo;

Timer printTimer(printDelay);

int open = 1;
int close = 2;
int off = 3;
int state = off;

void setup()
{
  Serial.begin(9600);
  jawServo.attach();

}

void loop()
{
  // Stop servo
  // jawServo.writeMicroseconds(servoStop);
  // delay(2000);
  // Get Pot Value
  linearPotVoltageADC = analogRead(linearPotPin);
  Serial.print("Initial linearPotVoltageADC:   ");
  Serial.println(linearPotVoltageADC);

  if (buttonB.isPressed()){
    state = open;
  }
  
  
  // Move Jaw Down
  if(state == open){
  jawServo.writeMicroseconds(servoJawDown);

  while (linearPotVoltageADC > jawOpenPotVoltageADC)
  {
    linearPotVoltageADC = analogRead(linearPotPin);
    if (printTimer.isExpired()){
      Serial.print("linearPotVoltageADC:    ");
      Serial.println(linearPotVoltageADC);
    }
  }
  

  // Stop servo
  jawServo.writeMicroseconds(servoStop);
  state = close;
  }
  linearPotVoltageADC = analogRead(linearPotPin);
  Serial.print("Bottom linearPotVoltageADC Before Delay:    ");
  Serial.println(linearPotVoltageADC);
  delay(1000);
  linearPotVoltageADC = analogRead(linearPotPin);
  Serial.print("Bottom linearPotVoltageADC After Delay:     ");
  Serial.println(linearPotVoltageADC);
  delay(1000);


  // Move Jaw Up
  if(state == close){
  jawServo.writeMicroseconds(servoJawUp);
  int prevVal = 0;
  while (linearPotVoltageADC < jawClosedPotVoltageADC)
  {
    
    // Serial.println("hello");
    linearPotVoltageADC = analogRead(linearPotPin);
    if (printTimer.isExpired()){
      Serial.print("linearPotVoltageADC:     ");
      Serial.println(linearPotVoltageADC);
      if(abs(analogRead(linearPotPin) - prevVal) < threshold){
        Serial.println("obstucted");
        state = off;
        if(analogRead(linearPotPin) < jawClosedPotVoltageADC-50){;
          state = open;
        }
        
        break;
      }
      prevVal = analogRead(linearPotPin);
    }
    
  }
  
  // Stop servo
  jawServo.writeMicroseconds(servoStop);
  }
  if(state == off){
    // Stop servo
    Serial.println("turned the servo off");
    jawServo.writeMicroseconds(servoStop);
    state = 5;
  }
  
  linearPotVoltageADC = analogRead(linearPotPin);
  Serial.print("Final linearPotVoltageADC Before Delay:      ");
  Serial.println(linearPotVoltageADC);
  delay(1000);
  linearPotVoltageADC = analogRead(linearPotPin);
  Serial.print("Final linearPotVoltageADC After Delay:      ");
  Serial.println(linearPotVoltageADC);
  delay(1000);
  
  
  // hi whatever
  // Stop servo
  // jawServo.writeMicroseconds(servoStop);
}

