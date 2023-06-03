#include <Servo.h>

Servo Gripper;  // create servo object to control a servo
Servo Arm;  // create servo object to control a servo
Servo Yaw; 

int GripperClose = 150; 
int GripperOpen = 255; 
int ArmPosition = 150; 
int YawPosition = 200 ; 
int GripperVal;
int ArmVal;
int YawVal;

void setup() {
  Serial.begin(9600);
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);  // attaches the servo on pin 22 to the servo object
  Yaw.attach(21);  
}

void loop() 
{
  // read the potentiometer value
  GripperVal = analogRead(A1); //Analog PIN A1
  ArmVal = analogRead(A2); //Analog PIN A2
  YawVal = analogRead(A3); //Analog PIN A3
  // map the potentiometer to gripper open close
  int GripPosition = map(GripperVal, 0, 1023, GripperClose, GripperOpen); 
  int ArmPosition = map(ArmVal, 0, 1023, GripperClose, GripperOpen);
  int YawPosition = map(YawVal, 0, 1023, GripperClose, GripperOpen);
  // mapping actual position for PWM signals
  int val1 = map(GripPosition, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  int val2 = map(ArmPosition, 0, 300, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  int val3 = map(YawPosition, 0, 300, 0, 180);

  Gripper.write(val1); // sets the servo position according to the scaled value
  Arm.write(val2); // sets the servo position according to the scaled value
  Yaw.write(val3);

  Serial.println(val1);
  Serial.println(val2);
  Serial.println(val3);
  
  delay(100); // waits for the servo to get there
}
