//PIN SETUP /////////////////////////////////////////
#include <Servo.h>
#define IR 4 
#define GS0 8
#define GS1 9
#define GS2 10
#define GS3 11
#define G_SensorOut 12

int GredFreq = 0;
int GgreenFreq = 0;
int GblueFreq = 0;

int GredCol = 0;
int GgreenCol = 0;
int GblueCol = 0;


Servo Gripper;  // create servo object to control a servo
Servo Arm;  // create servo object to control a servo
Servo Yaw; 

int GripperVal = 150;  // range 100 to 150, 134 for large, 116 for small
int ArmVal = 8;       // range 0 to 130, 3 is the resitng/pickup/place postion, lift to 10-15
int YawVal = 90 ;      // range 40 to 140, 90 is the middle 

/*
int GripperClose = 150; 
int GripperOpen = 255; 
int ArmPosition = 150; 
int YawPosition = 90 ; 
*/

//L298 Motor Controller Pin Assignment
// Motors A pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;
// Motors B pin connections
int enB = 5;
int in3 = 6;
int in4 = 7;

// LEFT and RIGHT TCS230 Color Sensor Pin Assignments

#define LS0 32
#define LS1 31
#define LS2 30
#define LS3 29
#define L_SensorOut 28

#define RS0 33
#define RS1 34
#define RS2 35
#define RS3 36
#define R_SensorOut 37

// LEFT and RIGHT Color Sensor Parameters

int LredFrequency = 0;
int LgreenFrequency = 0;
int LblueFrequency = 0;
int LredColor = 0;
int LgreenColor = 0;
int LblueColor = 0;
//int LstopColor = 0;

int RredFrequency = 0;
int RgreenFrequency = 0;
int RblueFrequency = 0;
int RredColor = 0;
int RgreenColor = 0;
int RblueColor = 0;
//int RstopColor = 0;

int GredFrequency = 0;
int GgreenFrequency = 0;
int GblueFrequency = 0;
int GredColor = 0;
int GgreenColor = 0;
int GblueColor = 0;
//int GstopColor = 0;

int desiredColor = 0;
//1 is red, 2 is blue
bool hasObject = false;
//switch to true when obj picked up
bool objRed = false;
bool objBlue = false;

bool doubleGreen = false;

bool large = false;
bool small = false;

#define bPin 16
bool buttonPressed = false;
int Gval = 150;

void setup() {
  // put your setup code here, to run once:
  //motor pinmodes
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Initial state - Turn of all the motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  // Port setup for LEFT color sensor pins
  
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(L_SensorOut, INPUT);

  /* Setting LEFT sensor frequency scaling to 20%
  
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);
  */

  // Setting LEFT sensor frequency scaling to 100% 
  // NOTE: there isn't a way to set freq in between 20 and 100
  
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);

  // Port setup for RIGHT color sensor pins
  
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(R_SensorOut, INPUT);
  
  /* Setting RIGHT sensor frequency scaling to 20%
  
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  */

  // Setting RIGHT sensor frequency scaling to 20%
  
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);

// Gripper Color Steup
  pinMode(GS0, OUTPUT);
  pinMode(GS1, OUTPUT);
  pinMode(GS2, OUTPUT);
  pinMode(GS3, OUTPUT);
  // Setting the sensorOut as an input
  pinMode(G_SensorOut, INPUT);
  digitalWrite(GS0,HIGH);
  digitalWrite(GS1,LOW);
  //Setting up IR pin as input 
  pinMode(IR,INPUT);

  doubleGreen = false;
  hasObject = false;
  objRed = true;
  objBlue = false;
  small = false;
  large = false;

  Serial.begin(115200);
  
  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);  // attaches the servo on pin 22 to the servo object
  Yaw.attach(21);  
  pinMode(bPin, INPUT); 
  Gripper.write(150);  
  Arm.write(5);  
  Yaw.write(94);
}

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

void movRIGHT(int speed) // right wheels locked
{
  int PWMval = (255*speed/100)*1.13;
  int RPWMval = (255*speed/100);  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left wheel clockwise and right wheel in anti-clockwise direction - Spin counter-clockwise/Turn left
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
}

void movLEFT(int speed) // correct might need to scale left/right side to account for drift
{
  int PWMval = (255*speed/100)*1.3;
  int RPWMval = (255*speed/100);  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH); 
}

void movFWD(int speed) // correct
{
  int PWMval = (255*speed/100)*1.13;
  int RPWMval = (255*speed/100);  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);  
}

void movBWD(int speed) // turning r (right wheels locked)
{
  int PWMval = (255*speed/100)*1.13;
  int RPWMval = (255*speed/100);  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left and right wheels in clockwise direction - Move forward
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

void ReadColorSensors()
{
  // Setting RED (R) filter photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,LOW);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,LOW);
  digitalWrite(GS2,LOW);
  digitalWrite(GS3,LOW);
  
  // Reading the output frequency
  
  RredFrequency = pulseIn(R_SensorOut, LOW);
  LredFrequency = pulseIn(L_SensorOut, LOW);
  GredFrequency = pulseIn(G_SensorOut, LOW);

  // Remaping the value of the RED (R) frequency from 0 to 255
  
  RredColor = map(RredFrequency, 79, 182, 255, 0);
  LredColor = map(LredFrequency, 88, 231, 255, 0); 
  GredColor = map(GredFrequency, 23, 150, 255, 0);
  delay(5);
  // Setting GREEN (G) filtered photodiodes to be read
  
  digitalWrite(RS2,HIGH);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,HIGH);
  digitalWrite(LS3,HIGH);
  digitalWrite(GS2,HIGH);
  digitalWrite(GS3,HIGH);
  
  // Reading the output frequency
  
  RgreenFrequency = pulseIn(R_SensorOut, LOW);
  LgreenFrequency = pulseIn(L_SensorOut, LOW);
  GgreenFrequency = pulseIn(G_SensorOut, LOW);
  
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  
  RgreenColor = map(RgreenFrequency, 87, 210, 255, 0);
  LgreenColor = map(LgreenFrequency, 96, 265, 255, 0);
  GgreenColor = map(GgreenFrequency, 27, 144, 255, 0);

  // Setting BLUE (B) filtered photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,HIGH);
  digitalWrite(GS2,LOW);
  digitalWrite(GS3,HIGH);
  // Reading the output frequency
  
  RblueFrequency = pulseIn(R_SensorOut, LOW);
  LblueFrequency = pulseIn(L_SensorOut, LOW);
  GblueFrequency = pulseIn(G_SensorOut, LOW);
  
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  
  RblueColor = map(RblueFrequency, 72, 168, 255, 0);
  LblueColor = map(LblueFrequency, 73, 191, 255, 0);
  GblueColor = map(GblueFrequency, 25, 162, 255, 0);
}

bool LseesRed(){
  if (LredColor > LgreenColor && LredColor > LblueColor && LredColor > 110 && LredColor < 220 && LgreenColor < 140){
    return true;
  }
  else {
    return false;
  }
}

bool RseesRed(){
  if (RredColor > RgreenColor && RredColor > RblueColor && RredColor > 110 && RredColor < 220 && RgreenColor < 140){
    return true;
  }
  else {
    return false;
  }
}

bool LseesGreen(){
  if (LgreenColor > LredColor && LgreenColor > 100 && LgreenColor < 190 && LblueColor<130 && LgreenColor > LblueColor){
    return true;
  }
  else {
    return false;
  }
}

bool RseesGreen(){
  if (RgreenColor > RredColor && RgreenColor > 100 && RgreenColor < 190 && RblueColor<130 && LgreenColor > LblueColor){
    return true;
  }
  else {
    return false;
  }
}

bool LseesBlue(){
  if (LblueColor > LgreenColor && LblueColor > LredColor && LblueColor > 150 && LgreenColor < 240){
    return true;
  }
  else {
    return false;
  }
}

bool RseesBlue(){
  if (RblueColor > RgreenColor && RblueColor > RredColor && RblueColor > 150 && RgreenColor < 240){
    return true;
  }
  else {
    return false;
  }
}

bool GseesRed(){
  if (GredColor > 220){
    return true;
  }
  else {
    return false;
  }
}

bool GseesBlue(){
  if (GblueColor > 220){
    return true;
  }
  else{
    return false;
  }
}

bool LseesWhite(){
  if (LredColor > 120 && LgreenColor > 120 && LblueColor > 120){
    return true;
  }
  else {
    return false;
  }
}

bool RseesWhite(){
  if (RredColor > 120 && RgreenColor > 120 && RblueColor > 120){
    return true;
  }
  else {
    return false;
  }
}

bool LseesBlack(){
  if (LredColor < 80 && LgreenColor < 80 && LblueColor < 80){
    return true;
  }
  else {
    return false;
  }
}

bool RseesBlack(){
  if (RredColor < 80 && RgreenColor < 80 && RblueColor < 80){
    return true;
  }
  else {
    return false;
  }
}

void loop() {
 ReadColorSensors();
  // put your main code here, to run repeatedly:
 Serial.print("RG: ");
 Serial.println(RgreenColor) ;
 Serial.print("RB: ");
 Serial.println(RblueColor) ;
 Serial.print("RR: ");
 Serial.println(RredColor) ;
 Serial.print("LG: ");
 Serial.println(LgreenColor) ;
 Serial.print("LB: ");
 Serial.println(LblueColor) ;
 Serial.print("LR: ");
 Serial.println(LredColor) ;
 Serial.print("GG: ");
 Serial.println(GgreenColor) ;
 Serial.print("GB: ");
 Serial.println(GblueColor) ;
 Serial.print("GR: ");
 Serial.println(GredColor) ;
 Serial.print("doubleGreen?: ");
 Serial.print(LseesGreen());
 Serial.print(", ");
 Serial.print(RseesGreen());
 Serial.print(", ");
 Serial.println(doubleGreen);
 Serial.print("Red? Small?: ");
 Serial.print(objRed);
 Serial.print(", ");
 Serial.println(small);


if (hasObject == false && doubleGreen == false){
  //drive from start to pickup
  Arm.write(8);
  Gripper.write(150); //116 small, 140 large
  Yaw.write(94); //94 middle
  delay(1000);

  movFWD(50);
  delay(2500);

  stop();
  delay(10);
   //pickup with gripper when code is set?
  while (buttonPressed == false){
    Gval = Gval-1;
    Gripper.write(Gval);
    if (digitalRead(bPin) == 1){
      buttonPressed = true;
      Serial.println(Gval);
    }
    delay(25);
  }
  if (buttonPressed == true){
    Arm.write(15);
  }
  // check size
  if (Gval < 120){
    small = true;
  }
  else {
    large = true;
  }
  //check color
  if (GseesRed()){
    objRed = true;
  }
  else if (GseesRed()){
    objRed = true;
  }

  //Find red/blue path

  //while (!LseesGreen() && !RseesGreen()){
    movBWD(60);
    delay (4700);
  //}
  if (objRed == true){
    movRIGHT(50);
    delay(3950);
    //while (!RseesRed()){
      movFWD(64);
      delay(7500);
    //}
    //movFWD(64);
    //delay(300);
    movRIGHT(50);
    delay(2950); 
  }
  else{
    movLEFT(50);
    delay(3050);
    //while (!LseesBlue()){
      movFWD(64);
      delay(7550);
    //}
    //movFWD(64);
    //delay(300);
    movLEFT(50);
    delay(2300);
  }

  movFWD(64);
  delay(250);
  stop(); // comment out delay when testing
  delay(5);
  hasObject = true;
} 

if (hasObject == true && doubleGreen == false){
    {
    //follow Red
    if (objRed && !doubleGreen){
      if (LseesRed() && !RseesRed()){
        Serial.print("RIGHT\n"); 
        stop();
        //movFWD(79);     // move FWD
        //delay(1200);   // duration of foward movment 
        movRIGHT(79);   // turn RIGHT  
        delay(150);    // duration of right turn for (almost) 90* turn is 1405?
        movFWD(79);
        delay(50);
        }

      if (!LseesRed() && RseesRed()){
        Serial.print("LEFT\n");
        stop();    
        //movFWD(79);      // move FWD.
        //delay(1200) ;    // duration of foward movement 
        movLEFT(79);     // turn LEFT  
        delay(150);     // duration of left turn  (1400 turn was short of 90*)
        stop();
        movFWD(79);
        delay(50);
        }

      if (LseesGreen() && RseesGreen()){
        doubleGreen = true;
      }

      else {movFWD(90);
        delay(100);}
    }
    
    
    //follow Blue
    else{
      if (!LseesBlue() && RseesBlue()){
        Serial.print("RIGHT\n"); 
        stop();
        delay(5);
        //movFWD(79);     // move FWD
        //delay(1200);   // duration of foward movment 
        movLEFT(79);   // turn RIGHT  
        delay(150);    // duration of right turn for (almost) 90* turn is 1405?
        stop();
        movFWD(79);
        delay(50);
      }

      if (LseesBlue() && !RseesBlue()){
        Serial.print("LEFT\n");
        stop();
        delay(5);    
        //movFWD(79);      // move FWD.
        //delay(1200) ;    // duration of foward movement 
        movRIGHT(79);     // turn LEFT  
        delay(150);     // duration of left turn  (1400 turn was short of 90*)
        stop();
        movFWD(79);
        delay(50);
      }

    if (LseesGreen() && RseesGreen()){
      doubleGreen = true;
      stop();
      delay(10);
    }

    else {
      movFWD(90);
      delay(10);
    }
  }
} 

/*
*
*
*    HARD CODE LAST SECTION (PLACEMENT)
*
*/

  if (doubleGreen){ //small is closest to accurate need to edit large
    if (large) {
      delay(200);
      movFWD(80);
      delay(5000);

      movLEFT(80);
      delay(2000);

      movFWD(80);
      delay(2750);

      movRIGHT(80);
      delay(2400);

      movFWD(80);
      delay(5375);
      stop();
      delay(1000);
      //place obj with gripper
      Arm.write(6);
      Gripper.write(150);
      delay(500);
      movBWD(60);
      delay(500);
    }

    else if (small){
      delay(200);
      movFWD(80);
      delay(5000);

      movRIGHT(80);
      delay(2000);

      movFWD(80);
      delay(2600);

      movLEFT(80);
      delay(1700);

      movFWD(80);
      delay(4700);
      stop();
      delay(1000);
      //place obj with gripper
      Arm.write(6);
      Gripper.write(150);
      delay(500);
      movBWD(60);
      delay(500);
    }
  stop();
  delay(100000);
  doubleGreen = false;
  }
  }
}
