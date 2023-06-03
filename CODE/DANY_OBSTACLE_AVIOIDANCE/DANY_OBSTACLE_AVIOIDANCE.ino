/********************************************************************************
File Name: DANY_OBSTACLE_AVIODANCE.ino
Created by: Daniel Silva Rios
Date Created: 04/28/23
Input:  2x Color Sensor
.       1x IR Senor
.       1x Ultrasonic Sensor
.
Output: Motor Control
.
Description: This program takes in the varaity of sensors data and 
.            deterines the movement functionality need to steer away
.            from obstacles
.
Side Effects: When in the reversing section of the algrothim, if no 
.             opening is detected by the ultrasonic sensors then it 
.             will countiue to reverse in denfinitley 
.
Modifications: N/A
*****************************************************************************/

// DEFINE SETUP 
 #include <Servo.h>
 // Ultrasonic and IR Define setup UNDER HERE 
  #define RechoPin 41 // attach Echo of HC-SR04
  #define RtrigPin 40//attach Trig of HC-SR04
  #define LechoPin 39 // attach Echo of HC-SR04
  #define LtrigPin 38//attach Trig of HC-SR04
  long Rduration;
  int Rdistance;
  long Lduration;
  int Ldistance;

  #define IR 4 



 // LEFT AND RIGHT COLOR SENSORS DEFINE SETUP UNDER HERE 

  // RIGHT COLOR SENSOR
  #define RS0 33
  #define RS1 34
  #define RS2 35
  #define RS3 36
  #define RSOut 37
  // LEFT COLOR SENSOR
  #define LS0 32
  #define LS1 31
  #define LS2 30
  #define LS3 29
  #define LSOut 28

  // Stores frequency read by the photodiodes
  int RredFreq = 0;
  int RgreenFreq = 0;
  int RblueFreq = 0;
  int LredFreq = 0;
  int LgreenFreq = 0;
  int LblueFreq = 0;

  // Stores the red. green and blue colors
  int RredCol = 0;
  int RgreenCol = 0;
  int RblueCol = 0;
  int LredCol = 0;
  int LgreenCol = 0;
  int LblueCol = 0;



 

 // Motors Setup UNDER HERE 
  //L298 Motor Controller Pin Assignment
  // Right Motor pin connections
  int enA = 1;
  int in1 = 2;
  int in2 = 3;
  // Left motor pin connections
  int enB = 5;
  int in3 = 6;
  int in4 = 7;

 
 //ARM SETUP UNDER HERE

  #define BUZZER 27 
  Servo Gripper;  // create servo object to control a servo
  Servo Arm;      
  Servo Yaw;   

  int GripperVal = 150;  // range 100 to 150
  int ArmVal = 120;       // range 0 to 130     5 is the resitng postion 
  int YawVal = 90 ;      // range 40 to 140    90 is the middle 
  int WallCheck = 0 ;

  //int BoundaryCheck = 0 ; 


 
 //

void setup() {
 
  // ULTRASONIC SETUP 
  pinMode(RtrigPin, OUTPUT); 
  pinMode(RechoPin, INPUT);
  pinMode(LtrigPin, OUTPUT); 
  pinMode(LechoPin, INPUT);
  
  // Left and Right Color Sensor Setup 
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(RSOut, INPUT);
  pinMode(LSOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);

  //Setting up IR pin as input 
  pinMode(IR,INPUT);

  //Motor Pins Setup 
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

  pinMode(BUZZER, OUTPUT);

  //Arm System pins 
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);      
  Yaw.attach(21);  


  Serial.begin(115200);//  setup serial
 

}

void loop() {

  Gripper.write(GripperVal); 
  delay(100);                 // delay waits for servo to catch up 
  Arm.write(ArmVal); 
  delay(100);
  Yaw.write(YawVal);
  delay(100);

 bool ObstacleDetec = false ; 
 // RIGHT ULTRASONIC SENSORS SET UP 
   digitalWrite(RtrigPin, LOW);
   delayMicroseconds(2);
   // Sets the trigPin HIGH for 10 microseconds
   digitalWrite(RtrigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(RtrigPin, LOW);
   // Reads the echoPin, returns the sound wave in microseconds
   Rduration = pulseIn(RechoPin, HIGH);
   // Calculating the distance
   Rdistance = Rduration * 0.034 / 2; // Speed of sound wave divided by 2 (send & receive)

 
   ///////////////////// FOR RIGHT COLOR SENSOR /////////////////////////////
   // Setting RED (R) filtered photodiodes to be read
   digitalWrite(RS2,LOW);
   digitalWrite(RS3,LOW);
   // Reading the output frequency
   RredFreq = pulseIn(RSOut, LOW);
   // Remaping the value of the RED (R) frequency from 0 to 255
   RredCol = map(RredFreq, 79, 182, 255, 0);


   // Setting GREEN (G) filtered photodiodes to be read
   digitalWrite(RS2,HIGH);
   digitalWrite(RS3,HIGH);
   // Reading the output frequency
   RgreenFreq = pulseIn(RSOut, LOW);
   // Remaping the value of the GREEN (G) frequency from 0 to 255
   RgreenCol = map(RgreenFreq, 87, 210, 255, 0);


   // Setting BLUE (B) filtered photodiodes to be read
   digitalWrite(RS2,LOW);
   digitalWrite(RS3,HIGH);
   // Reading the output frequency
   RblueFreq = pulseIn(RSOut, LOW);
   // Remaping the value of the BLUE (B) frequency from 0 to 255
   RblueCol = map(RblueFreq, 72, 168, 255, 0);

   /////////////////////////////////////////////////////////////////////////

   ///////////////////// FOR LEFT COLOR SENSOR /////////////////////////////
   // Setting RED (R) filtered photodiodes to be read
   digitalWrite(LS2,LOW);
   digitalWrite(LS3,LOW);
   // Reading the output frequency
   LredFreq = pulseIn(LSOut, LOW);
   // Remaping the value of the RED (R) frequency from 0 to 255
   LredCol = map(LredFreq, 88, 231, 255, 0);

   // Setting GREEN (G) filtered photodiodes to be read
   digitalWrite(LS2,HIGH);
   digitalWrite(LS3,HIGH);
   // Reading the output frequency
   LgreenFreq = pulseIn(LSOut, LOW);
   // Remaping the value of the GREEN (G) frequency from 0 to 255
   LgreenCol = map(LgreenFreq, 96, 265, 255, 0);

   // Setting BLUE (B) filtered photodiodes to be read
   digitalWrite(LS2,LOW);
   digitalWrite(LS3,HIGH);
   // Reading the output frequency
   LblueFreq = pulseIn(LSOut, LOW);
   // Remaping the value of the BLUE (B) frequency from 0 to 255
   LblueCol = map(LblueFreq, 73, 191, 255, 0);


 //
  ObstacleDetec = digitalRead(IR);
  


if( ObstacleDetec == LOW ){
   //Serial.println("OBSTACLE IN FRONT OF ME ");
   //Serial.println("STOP MOVEMENT FUNCTION "); 
    stop();
    delay(10); 

   //Serial.println("TURN LEFT FUNCTION "); 
    movCCW(80); 
    delay(2000);   //  #1  this delay will cont how long to turn the vehicle 

   //Serial.println("STOP MOVEMENT FUNCTION "); 
    stop();
   WallCheck = 1 ; 
   delay(10);
   Rdistance = 0 ; 

 }
if ( Rdistance > 30 && WallCheck == 1 ){
 // Serial.println("STOP MOVMENT FUNCTION "); 
  stop();
  delay(10);

  //Serial.println("MOVE FOWARD FUNCTION "); 
  movFW(80);
  delay(500);   // this delay will contorl the time to get the vehclie to movein front of the gap 

  //Serial.println("MOVE TURN TOWARDS THE ULTRASONIC SIDE FUNCTION  RIGHT ") ; // this turn function will turn the corresponif sensor that detected gap
  movCW(80);
  delay(2000);  // #2  this time will control the libeing up to face foward 

  //Serial.println("STOP MOVMENT FUNCTION "); 
  stop();
  delay(10);  

   WallCheck = 0;
 }


if (LredCol > 200 && LgreenCol > 200 && LblueCol > 200 && RredCol > 200 && RgreenCol > 200 && RblueCol > 200  ){
  //Serial.println("STOP MOVMENT FUNCTION "); 
  stop();
  int reversecheck = 1 ;
  delay(10);

  digitalWrite(BUZZER, HIGH); 

  Serial.println(" REVERSE "); 
  while (reversecheck == 1 ){
  //READ ULTRA
   digitalWrite(RtrigPin, LOW);
   delayMicroseconds(2);
   // Sets the trigPin HIGH for 10 microseconds
   digitalWrite(RtrigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(RtrigPin, LOW);
   // Reads the echoPin, returns the sound wave in microseconds
   Rduration = pulseIn(RechoPin, HIGH);
   // Calculating the distance
   Rdistance = Rduration * 0.034 / 2; // Speed of sound wave divided by 2 (send & receive)
  movBW(80);

    if( Rdistance > 30 ){
      //Serial.print(Rdistance);    // CAN DELETE THESE PRINTS IN FINAL VERSION 
      //Serial.println("cm Right");
      //delay(1000);

      digitalWrite(BUZZER, LOW) ; 
      stop();
      movBW(80);
      delay(1600);
      stop();
      movCW(80);
      delay(2000);  //#3
      stop();
      reversecheck = 0;
      WallCheck = 0;
      //Serial.println(" REVERSE STOPED "); 
    }
  }



 }

else{
   movFW(80); 
  }
 }
////////////////END OF VOID LOOP ///////////////////////


/// MOVEMENT FUNCTIONS ////
//
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
	analogWrite(enA, PWMval);
	analogWrite(enB, RPWMval);
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



