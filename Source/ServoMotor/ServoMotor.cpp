#include <Servo.h> 
 
int servoPin = 2;

Servo servo; 

int angle = 0; // servo position in degrees 

void setup() 
{ 
    servo.attach(servoPin); 
} 


void loop() 
{ 
  // scan from 0 to 90 degrees
  for(angle = 0; angle < 90; angle++) 
  { 
    servo.write(angle); 
    delay(0); 
  } 
  // now scan back from 90 to 0 degrees
  for(angle = 90; angle > 0; angle--) 
  { 
    servo.write(angle); 
    delay(30); 
  } 
} 

