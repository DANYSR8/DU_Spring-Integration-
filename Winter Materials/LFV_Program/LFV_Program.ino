// Time-Step Degree of Vehicle Rotation

#define TimeStep_ms 1500

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
int LstopColor = 0;

int RredFrequency = 0;
int RgreenFrequency = 0;
int RblueFrequency = 0;
int RredColor = 0;
int RgreenColor = 0;
int RblueColor = 0;
int RstopColor = 0;

//L298 Motor Controller Pin Assignment

// LEFT Motor (LM) pin connections
int EnableLM = 1;
int LM_in1 = 2;
int LM_in2 = 3;

// RIGHT Motor (RM) pin connections
int EnableRM = 5;
int RM_in3 = 6;
int RM_in4 = 7;

bool looking = true; 

void setup()
{
  // Set all motor controller pins as outputs
	
  pinMode(EnableLM, OUTPUT);
  pinMode(EnableRM, OUTPUT);
  pinMode(LM_in1, OUTPUT);
  pinMode(LM_in2, OUTPUT);
  pinMode(RM_in3, OUTPUT);
  pinMode(RM_in4, OUTPUT);
	
  // Initial state - Turn off all the motors
	
  digitalWrite(LM_in1, LOW);
  digitalWrite(LM_in2, LOW);
  digitalWrite(RM_in3, LOW);
  digitalWrite(RM_in4, LOW);

  // Port setup for LEFT color sensor pins
  
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(L_SensorOut, INPUT);
  
  // Setting LEFT sensor frequency scaling to 20%
  
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);
  
  // Port setup for RIGHT color sensor pins
  
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(R_SensorOut, INPUT);
  
  // Setting RIGHT sensor frequency scaling to 20%
  
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  
  // Begins serial communication
  Serial.begin(115200);
}

void loop() 
{
  //////////////// PUT LFV CONTROL PROGRAM HERE ////////////////
  /////////// DANY's Code /////////////////////////////////////
  ////////////Function at very bottom of code ////////////////
    MovFWD(50);


  ReadColorSensors();
  if(looking){

  SearchMode() ; 
  Serial.print("SERACHING \n"); 

}
else{
   FollowMode(); 
  }




}

//////////////////////////////////////////////////////////////////////////////
// ReadColorSensors():  Read RIGHT and LEFT color sensors.
//////////////////////////////////////////////////////////////////////////////

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
  
  RredColor = map(RredFrequency, 48, 195, 255, 0);
  LredColor = map(LredFrequency, 45, 222, 255, 0); 
 
  // Setting GREEN (G) filtered photodiodes to be read
  
  digitalWrite(RS2,HIGH);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,HIGH);
  digitalWrite(LS3,HIGH);
  
  // Reading the output frequency
  
  RgreenFrequency = pulseIn(R_SensorOut, LOW);
  LgreenFrequency = pulseIn(L_SensorOut, LOW);
  
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  
  RgreenColor = map(RgreenFrequency, 61, 313, 255, 0);
  LgreenColor = map(LgreenFrequency, 55, 332, 255, 0);

  // Setting BLUE (B) filtered photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,HIGH);
  
  // Reading the output frequency
  
  RblueFrequency = pulseIn(R_SensorOut, LOW);
  LblueFrequency = pulseIn(L_SensorOut, LOW);
  
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  
  RblueColor = map(RblueFrequency, 52, 275, 255, 0);
  LblueColor = map(LblueFrequency, 52, 332, 255, 0);
}

// VEHICLE MOVEMENT SUBROUTINES

//////////////////////////////////////////////////////////////////////////////
// Stop():  Stop vehicle movement by stopping wheel rotation.
//////////////////////////////////////////////////////////////////////////////

void Stop() 
{
  // The PWM value is 0 for a modulation of 0%
  
  analogWrite(EnableLM, 0);
  analogWrite(EnableRM, 0);
  
  // Turn off all motors

  digitalWrite(LM_in1, LOW);
  digitalWrite(LM_in2, LOW);
  digitalWrite(RM_in3, LOW);
  digitalWrite(RM_in4, LOW); 
}

//////////////////////////////////////////////////////////////////////////////
// MovFWD( int speed ):  Right and Left wheels will rotate in the CW direction.
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void MovFWD( int speed )
{
  int  PWMval = 255*(speed/100);
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableLM, PWMval);
  analogWrite(EnableRM, PWMval);

  // Rotate Right and Left wheels clockwise (CW) 

  digitalWrite(LM_in1, HIGH);
  digitalWrite(LM_in2, LOW);
  digitalWrite(RM_in3, LOW);
  digitalWrite(RM_in4, HIGH); 
}

//////////////////////////////////////////////////////////////////////////////
// MovBKWD( int speed ):  Right and left wheels will rotate in the CCW direction.
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void MovBKWD( int speed )
{
  int  PWMval = 255*(speed/100);
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableLM, PWMval);
  analogWrite(EnableRM, PWMval);

  // Rotate Right and Left wheels counter-clockwise (CCW)  

  digitalWrite(LM_in1, LOW);
  digitalWrite(LM_in2, HIGH);
  digitalWrite(RM_in3, HIGH);
  digitalWrite(RM_in4, LOW); 
}

//////////////////////////////////////////////////////////////////////////////
// RotateRt( int speed ):  
//    CW rotation of vehicle -- Left wheel rotates forward; Right wheel
//    rotates backwards.  Each time this subroutine is called, causes the
//    vehicle to rotate in time-step increments.  The predefined value for
//    "TimeStep_ms" is the number of milli-seconds of rotation.  The angle
//    the vehicle will be facing is determined by how fast the vehicle is 
//    rotating and the duration of rotation (how long the vehicle has been)
//    rotating.
//
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void RotateRt( int speed )
{
  int  PWMval = 255*(speed/100);
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableLM, PWMval);
  analogWrite(EnableRM, PWMval);

  // Rotate Left wheel forward; Rotate Right wheel backward  
  
  digitalWrite(LM_in1, LOW);
  digitalWrite(LM_in2, HIGH);
  digitalWrite(RM_in3, LOW);
  digitalWrite(RM_in4, HIGH);

  delay(1500);
}

//////////////////////////////////////////////////////////////////////////////
// RotateLft( int speed ):  
//    CCW rotation of vehicle -- Left wheel rotates backward; Right wheel
//    rotates forward.  Each time this subroutine is called, causes the
//    vehicle to rotate in time-step increments.  The predefined value for
//    "TimeStep_ms" is the number of milli-seconds of rotation.  The angle
//    the vehicle will be facing is determined by how fast the vehicle is 
//    rotating and the duration of rotation (how long the vehicle has been
//    rotating).
//
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void RotateLft( int speed)
{
  int  PWMval = 255*(speed/100);
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableLM, PWMval);
  analogWrite(EnableRM, PWMval);

  // Rotate Left wheel backward; Rotate Right wheel forward  
  
  digitalWrite(LM_in1, HIGH);
  digitalWrite(LM_in2, LOW);
  digitalWrite(RM_in3, HIGH);
  digitalWrite(RM_in4, LOW);

  delay(TimeStep_ms); 
}

////////////////////////////////////////////////////////////////////////////
////////////////////////// Dany' s Functions Code /////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////SEARCH MODE ////////////////////////////////////////////////

void SearchMode()
{
Serial.println(RgreenColor) ;
Serial.println(RblueColor) ;
Serial.println(RredColor) ;

RotateRt(79);


if ( RgreenColor > 110 && RblueColor < 100 && RredColor < 100 ){

Serial.println(RgreenColor) ;
Serial.println(RblueColor) ;
Serial.println(RredColor) ;
looking = false; 

}

}

////////// FOLLOW MODE //////////////////////////

void FollowMode(){
looking = false; 
Stop(); 
delay(1000); 
ReadColorSensors();
Serial.println(LgreenColor) ;
Serial.println(LblueColor) ;
Serial.println(LredColor);

if(LredColor < 20 && RredColor < 20 && LgreenColor < 20 && RgreenColor < 20 && LblueColor < 20 && RblueColor < 20){
Serial.print("STOP\n"); 
Stop(); 
looking = true ;   
}

else if ( RgreenColor > 110 &&  RblueColor < 100 && RredColor < 100){
Serial.print("RIGHT\n"); 
MovFWD(79);     // move faoward for a certin amount of time 
delay(1000) ; 
RotateRt(79);   //turn right motor comand number inside is the speed value  
delay(1000);      // this will be change to know how long the turn is 
Stop();
}

else if (LgreenColor > 110 && LredColor < 100){
  Serial.print("LEFT\n"); 
MovFWD(79);     
delay(1000) ; 
RotateLft(79);     
delay(1000);      
Stop();
}

else {
  Serial.print("FOWARD\n"); 
MovFWD(79); 

}

}


