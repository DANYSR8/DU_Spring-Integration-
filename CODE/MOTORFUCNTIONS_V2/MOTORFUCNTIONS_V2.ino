//L298 Motor Controller Pin Assignment
// Right Motor pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;
// Left motor pin connections
int enB = 5;
int in3 = 6;
int in4 = 7;


#include <Servo.h>

Servo Gripper;  // create servo object to control a servo
Servo Arm;      
Servo Yaw;   

int GripperVal = 125;  // range 100 to 150
int ArmVal = 18;       // range 0 to 130     5 is the resitng postion 
int YawVal = 90 ;      // range 40 to 140    90 is the middle 



void setup()
{
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);      
  Yaw.attach(21);  

	// Set all motor controller pins as outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Initial state - Turn off all the motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void loop() 
{

  Gripper.write(GripperVal); 
  delay(100);                 // delay waits for servo to catch up 
  Arm.write(ArmVal); 
  delay(100);
  Yaw.write(YawVal);
  delay(100);


/* ALL MOEVMENT CHECK UNDER HERe  
  movFW(80); // CHECKED 
  delay(1000); 

  movBW(80); // CHECKED 
  delay(1000); 

  movCW(80); // CHECKED 
  delay(1000); 

  movCCW(80); // CHECKED 
  delay(5000); 

*/

// /* RAMP COMMAD UBNDER HERE 
  movFW(80); 
 // */

// TESTING Diffenet Turns 
 // movCW(80); 

}


// stop any movement
void stop()
{
  // move forward with 0% speed - For PWM value 0
	analogWrite(enA, 0);
	analogWrite(enB, 0); 
	// Turn off all the motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);  
}


void movFW(int speed)
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1;  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, RPWMval);
  // Rotate left and right wheels in clockwise direction - Move forward
  digitalWrite(in1, LOW);  // Right Wheels
	digitalWrite(in2, HIGH); // Right wheels
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
}


void movBW(int speed)
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1;
  // move backward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);  
}


void movCW(int speed)
{
  int PWMval = 255*speed/100;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, PWMval);
  // Rotate left wheel clockwise and right wheel in anti-clockwise direction - Spin clockwise/Turn right
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  delay(1000);
}


void movCCW(int speed)
{
  int PWMval = 255*speed/100;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, PWMval);
  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  delay(1000); // turn left for 1 second  
}
