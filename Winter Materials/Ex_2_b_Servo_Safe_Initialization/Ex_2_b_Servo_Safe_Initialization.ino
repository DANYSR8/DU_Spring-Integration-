#include <Servo.h>

Servo Gripper;  // create servo object to control a servo
Servo Arm;  // create servo object to control a servo

int actualPosition1 = 150; 
int actualPosition2 = 150; 
int PWMSignal;

void setup() {
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);  // attaches the servo on pin 22 to the servo object
}

void loop() {

  // safe position - middle of range of motion 0-300 degrees
  actualPosition1 = 150; 
  actualPosition2 = 150;

  // mapping actual position for PWM signals
  int val1 = map(actualPosition1, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  int val2 = map(actualPosition2, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)

  Gripper.write(val1); // sets the servo position according to the scaled value
  Arm.write(val2); // sets the servo position according to the scaled value
  delay(100); // waits for the servo to get there
}
