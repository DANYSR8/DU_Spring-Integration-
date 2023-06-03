#include <Servo.h>

Servo Gripper;  // create servo object to control a servo
Servo Arm;  // create servo object to control a servo

int actualPosition1 = 0; 
int actualPosition2 = 300; 
int PWMSignal;
int change1 = 0;
int change2 = 0;

void setup() {
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);  // attaches the servo on pin 22 to the servo object
}

void loop() {
  actualPosition1 = actualPosition1 + change1;
  actualPosition2 = actualPosition2 + change2;
  if(actualPosition1<30) // min angular position 0 degree
  {
    change1 = 30;
  }
  if(actualPosition1>270) // max angular position 300 degrees
  {
    change1 = -30;
  }    

  if(actualPosition2<30) // min angular position 0 degree
  {
    change2 = 30;
  }
  if(actualPosition2>270) // max angular position 300 degrees
  {
    change2 = -30;
  }   

  // mapping actual position for PWM signals
  int val1 = map(actualPosition1, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  int val2 = map(actualPosition2, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)

  Gripper.write(val1); // sets the servo position according to the scaled value
  Arm.write(val2); // sets the servo position according to the scaled value
  delay(100); // waits for the servo to get there
}
