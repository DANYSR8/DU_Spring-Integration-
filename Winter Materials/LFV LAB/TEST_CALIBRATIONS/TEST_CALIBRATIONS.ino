///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PIN SETUP /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// TCS230 or TCS3200 pins wiring to Arduino
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void setup() {

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// SENSOR SETUP ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

  // Setting the outputs
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
  
  // Begins serial communication
  Serial.begin(115200);


  int RRedMax = 0 ; 
  int RRedMin = 1023 ; 

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

}

void loop() {
  
    ///////////////////// FOR RIGHT COLOR SENSOR /////////////////////////////
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,LOW);
  // Reading the output frequency
  RredFreq = pulseIn(RSOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  RredCol = map(RredFreq, 0, 40, 0, 255);
  // Printing the RED (R) value
  //Serial.print("RR = ");
  //Serial.print(RredFreq);

  if(RredFreq < RRedMin){
    RRedMin = RredFreq ; 
  }
    if(RredFreq > RRedMax){
    RRedMax = RredFreq ; 
  }

  delay(10);

  Serial.print(RRedMax);
  Serial.print(RRedMin);   

}
