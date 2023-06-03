//L298 Motor Controller Pin Assignment
// Motor A pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;
// Motor B pin connections
int enB = 5;
int in3 = 6;
int in4 = 7;

//int timeMS = 1000;
//int speed = 50 ; 

void setup()
{
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
  /*
  movFW(79); // 
  delay(1500); // time for move fw
  movCW(79); //
  delay(1500); // time for turn left
  movCCW(79); // 
  delay(1500); // time for turn right
  movBW(79); //
  delay(1500); // time for move bw
  stop();
  delay(5000); // delay whole program for 10s

*/

    movBW(95); // move fwd 
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

// move forward
void movFW(int speed)
{
  int PWMval = 255*speed/100;
   int RPWMval = (255*speed/100)*1.2;  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, RPWMval);
  // Rotate left and right wheels in clockwise direction - Move forward
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

// move backward
void movBW(int speed)
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1;
  // move backward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);  
}

// turn left
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

// turn right
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
