//PIN SETUP /////////////////////////////////////////

//L298 Motor Controller Pin Assignment
// RIGHT MOTORS pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;
// LEFT Motors pin connections
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

// LEFT and RIGHT Color Sensor Parameters UNDER HERE 


 int LredFrequency = 0;
 int LgreenFrequency = 0;
 int LblueFrequency = 0;
 int LredColor = 0;
 int LgreenColor = 0;
 int LblueColor = 0;
 int LstopColor = 0;

 int RredFrequency = 0;
 int RgreenFrequency = 0;
 int RblueFrequency = 0;
 int RredColor = 0;
 int RgreenColor = 0;
 int RblueColor = 0;
 int RstopColor = 0;

 int GredFrequency = 0;
 int GgreenFrequency = 0;
 int GblueFrequency = 0;
 int GredColor = 0;
 int GgreenColor = 0;
 int GblueColor = 0;
 int GstopColor = 0;

 int desiredColor = 0;

//1 is red, 2 is blue
bool hasObject = false;
//switch to true when obj picked up


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

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
  digitalWrite(LS1,HIGH);

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

  // Setting RIGHT sensor frequency scaling to 100%
  
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,HIGH);
  
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
/*
void leftWheels(int speed) // ?
{ 
  int PWMval = 255*speed/100;
  analogWrite(enA, PWMval);
  analogWrite(enB, 0);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
*/

void movRIGHT(int speed) // right wheels locked
{
  int PWMval = 255*speed/100;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, PWMval);
  // Rotate left wheel clockwise and right wheel in anti-clockwise direction - Spin counter-clockwise/Turn left
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
}

void movLEFT(int speed) // correct might need to scale left/right side to account for drift
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
}

void movFWD(int speed) // correct
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1;
  // move backward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval); //set left wheels
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);  
}

void movBWD(int speed) // turning r (right wheels locked)
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1;  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
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
  
  // Reading the output frequency
  
  RredFrequency = pulseIn(R_SensorOut, LOW);
  LredFrequency = pulseIn(L_SensorOut, LOW);
  
  // Remaping the value of the RED (R) frequency from 0 to 255
  
  RredColor = map(RredFrequency, 79, 182, 255, 0);
  LredColor = map(LredFrequency, 88, 231, 255, 0); 
 
  // Setting GREEN (G) filtered photodiodes to be read
  
  digitalWrite(RS2,HIGH);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,HIGH);
  digitalWrite(LS3,HIGH);
  
  // Reading the output frequency
  
  RgreenFrequency = pulseIn(R_SensorOut, LOW);
  LgreenFrequency = pulseIn(L_SensorOut, LOW);
  
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  
  RgreenColor = map(RgreenFrequency, 87, 210, 255, 0);
  LgreenColor = map(LgreenFrequency, 96, 265, 255, 0);

  // Setting BLUE (B) filtered photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,HIGH);
  
  // Reading the output frequency
  
  RblueFrequency = pulseIn(R_SensorOut, LOW);
  LblueFrequency = pulseIn(L_SensorOut, LOW);
  
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  
  RblueColor = map(RblueFrequency, 72, 168, 255, 0);
  LblueColor = map(LblueFrequency, 73, 191, 255, 0);
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
  if (LgreenColor > LredColor && LgreenColor > LblueColor && LgreenColor > 40 && LgreenColor < 190){
    return true;
  }
  else {
    return false;
  }
}

bool RseesGreen(){
  if (RgreenColor > RredColor && RgreenColor > RblueColor && RgreenColor > 40 && RgreenColor < 190){
    return true;
  }
  else {
    return false;
  }
}

bool LseesBlue(){
  if (LblueColor > LgreenColor && LblueColor > LredColor && LblueColor > 120 && LblueColor < 210 && LgreenColor < 200){
    return true;
  }
  else {
    return false;
  }
}

bool RseesBlue(){
  if (RblueColor > RgreenColor && RblueColor > RredColor && RblueColor > 120 && RblueColor < 210 && RgreenColor < 200){
    return true;
  }
  else {
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

 if (hasObject == false){
   //drive from start to pickup
   movFWD(40);
   delay(700);
   //pickup with gripper when code is set?
   //180 turn
   movLEFT(50);
   delay(600);
   //drive to start line (green detected)
   while (!RseesGreen){
     movFWD(40);
   }
   movRIGHT(50);
   delay(600);
   hasObject = true;
 } 


else{
//follow Blue

 if (!LseesBlue() && RseesBlue()){
 Serial.print("RIGHT\n"); 
 stop();
 delay(10);
 //movFWD(79);     // move FWD
 //delay(1200);   // duration of foward movment 
 movRIGHT(79);   // turn RIGHT  
 delay(105);    // duration of right turn for (almost) 90* turn is 1405?
 stop();
 movFWD(79);
 delay(100);
 }

 if (LseesBlue() && !RseesBlue()){
 Serial.print("LEFT\n");
 stop();
 delay(10);    
 //movFWD(79);      // move FWD.
 //delay(1200) ;    // duration of foward movement 
 movLEFT(79);     // turn LEFT  
 delay(105);     // duration of left turn  (1400 turn was short of 90*)
 stop();
 movFWD(79);
 delay(100);
 }

 else{
 Serial.print("FOWARD\n"); 
 movFWD(50);    
 }

}
}

