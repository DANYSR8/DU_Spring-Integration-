/********************************************************************************

  File Name:       DANY'sLFV.ino
  Created by:      Daniel Silva Rios  
  Date Created:    01/23/23
  Input:           2x Color Sensor 
  Output:          2x Motor Control 
  Description:     This program takes in color sensors values and  
 .                 determines what avtion the motors should take  
  Side Effects:    N/A
  Modifications:   N/A


  *****************************************************************************/

//PIN SETUP /////////////////////////////////////////

//L298 Motor Controller Pin Assignment
// Motor A pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;
// Motor B pin connections
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
int LstopColor = 0;

int RredFrequency = 0;
int RgreenFrequency = 0;
int RblueFrequency = 0;
int RredColor = 0;
int RgreenColor = 0;
int RblueColor = 0;
int RstopColor = 0;

bool looking = true; 


//////////////////////////////////////////////////////////////////

void setup() {
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

void loop() {

 ReadColorSensors();                   // Read the color sensors values each time it goes through the loop 

 if(looking){                          // boolean value that with either be true or false, this controls when to enter serach mode 
  Serial.print("SERACHING \n");
  SearchMode();
 }else{                                // if the search mode condition is not met the LFV will be put in follow mode 
  FollowMode();
 }


}



// GIVEN/BASIC FUNCTIONS NEEDED  /////////////////////////////////////////////////////

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

void movLEFT(int speed)
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

void movRIGHT(int speed)
{
  int PWMval = 255*speed/100;
  int RPWMval = (255*speed/100)*1.2;
  // move backward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, RPWMval);
	analogWrite(enB, PWMval);
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);  
}

void movFWD(int speed)
{
  int PWMval = (255*speed/100) * 0.99;  // scaleing values for both motors to keep the LFV moving in a stright path 
  int CPWMval = (255*speed/100) * 1.05;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, CPWMval);
  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  delay(1000); // turn left for 1 second  
}

void movBWD(int speed)
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
  
  RblueColor = map(RblueFrequency, 52, 332, 255, 0);
  LblueColor = map(LblueFrequency, 52, 332, 255, 0);
}


// COUSTOM FUNCTIONS//////////////////////////////////////////////////////
void SearchMode()
{

// RED PATH SEARCH
 
 movRIGHT(50);  // LFV will turn right in place  until if statement is met 

 if ( RredColor > 160 && RgreenColor < 150 &&  RblueColor < 150 ){ 

 // once the path is detected the LFV will stop, move right to keep the line 
 //in the middle and set the boolaing to stop the LFV entering Serach mode 
  stop();          
  delay(500);
  movRIGHT(79);
  delay(200);
  stop();
  looking = false; 
  
 }

 
 

// BLUE PATH 
 /*
 movRIGHT(50);    // LFV will turn right in place  until if statement is met 
 

 if ( RblueColor > 125 && RgreenColor < 100 && RredColor < 100  ){ 

  // once the path is detected the LFV will stop, move right to keep the line 
 //in the middle and set the boolaing to stop the LFV entering Serach mode 
  stop();
  delay(500);
  movRIGHT(79);
  delay(200);
  stop();
  delay(500);
  looking = false; 
 }

 */
}

void FollowMode()
{

// RED PATH FOLOW 

  
 looking = false; 


 // Check for black/stop condition 
 if(LredColor < 50 && RredColor < 50 && LgreenColor < 50 && RgreenColor < 50 && LblueColor < 50 && RblueColor < 50){
 Serial.println("STOP"); 
 stop();
 delay(2000);
 movFWD(40);
 delay(1000);
 looking = true ;   
 }

 // condtion for turning right 
 if (RredColor > 150 &&  RredColor > RgreenColor &&  RredColor > RblueColor ){
 Serial.print("RIGHT\n"); 
 stop();
 delay(500);
 movFWD(79);     // move FWD
 delay(1150) ;   // duration of foward movment 
 movRIGHT(79);   // turn RIGHT  
 delay(1450);    // duration of right turn 
 stop();
 }

 // condition for turning left 
 if (LredColor > 150 && LredColor > LgreenColor  && LredColor > LblueColor ){
 Serial.print("LEFT\n");
 stop();
 delay(500);    
 movFWD(79);      // move FWD
 delay(1150) ;    // duration of foward movement 
 movLEFT(79);     // turn LEFT  
 delay(1450);     // duration of left turn  
 stop();
 }
 
 // always moving foward if none of the conditions are met 
 else{
 Serial.print("FOWARD\n"); 
 movFWD(20);    
 }
  
  
 
// BLUE PATH FOLLOW 
  
  /*
  Serial.println(RgreenColor) ;
  Serial.println(RblueColor) ;
  Serial.println(RredColor) ;
  Serial.println(LgreenColor) ;
 Serial.println(LblueColor) ;
 Serial.println(LredColor) ;
 
 // intial stop once color is detected  
 looking = false; 
 //stop();
 //delay(1000);  // stop duration 
 //movRIGHT(79); // correction right turn  
 //delay(100);   // delay of correction turn 


 // Check for black/stop condition 
 if(LredColor < 20 && RredColor < 20 && LgreenColor < 20 && RgreenColor < 20 && LblueColor < 20 && RblueColor < 20){
 Serial.println("STOP"); 
 stop();
 delay(2000);
 movFWD(40);
 delay(1000);
 looking = true ;   
 }

 // condtion for turning right 
 if (  RblueColor > 70 &&  RblueColor > RredColor   ){
 Serial.print("RIGHT\n"); 
 stop();
 delay(500);
 movFWD(79);     // move FWD
 delay(1200) ;   // duration of foward movment 
 movRIGHT(79);   // turn RIGHT  
 delay(1400);    // duration of right turn 
 stop();
 }

 // condition for turning left 
 if ( LblueColor > 70 &&  LblueColor > LredColor  ){
 Serial.print("LEFT\n");
 stop();
 delay(500);    
 movFWD(79);      // move FWD.
 delay(1200) ;    // duration of foward movement 
 movLEFT(79);     // turn LEFT  
 delay(1500);     // duration of left turn  (was 1400 turn was short)
 stop();
 }
 
 // always moving foward if none of the conditions are met 
 else{
 Serial.print("FOWARD\n"); 
 movFWD(30);    
 }
 
 */

}


