#include <Servo.h> 

Servo Gripper;  // create servo object to control a servo
Servo Arm;      
Servo Yaw;   

int GripperVal = 115;  // range 100 to 150
int ArmVal = 0;       // range 0 to 130     5 is the resitng postion 
int YawVal = 90 ;      // range 40 to 140    90 is the middle 

//GRIPPER COLOR SENSORS SETUP 

// GS 
#define GS0 8
#define GS1 9
#define GS2 10
#define GS3 11
#define GSOut 12

// Stores frequency read by the photodiodes
int GredFreq = 0;
int GgreenFreq = 0;
int GblueFreq = 0;

// Stores the red. green and blue colors
int GredCol = 0;
int GgreenCol = 0;
int GblueCol = 0;

void setup() {



  Gripper.attach(23);  // attaches the servo on pin 23 to the servo object
  Arm.attach(22);      
  Yaw.attach(21);  

    // Setting the outputs
  pinMode(GS0, OUTPUT);
  pinMode(GS1, OUTPUT);
  pinMode(GS2, OUTPUT);
  pinMode(GS3, OUTPUT);
  // Setting the sensorOut as an input
  pinMode(GSOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(GS0,HIGH);
  digitalWrite(GS1,LOW);

Serial.begin(115200);

}

void loop() {

  Gripper.write(GripperVal); 
  delay(100);                 // delay waits for servo to catch up 
  Arm.write(ArmVal); 
  delay(100);
  Yaw.write(YawVal);
  delay(100);

  /// ELLIOT & STEVEN ALGETHIM CODE UNDER HERE 




  ///ELLITO &STEVEN ALGROTHIM ENDS HERE 



/////////////////////////////////////////////
///////////////////// FOR GRIPPER COLOR SENSOR /////////////////////////////
   // Setting RED (R) filtered photodiodes to be read
  digitalWrite(GS2,LOW);
  digitalWrite(GS3,LOW);
  // Reading the output frequency
  GredFreq = pulseIn(GSOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  GredCol = map(GredFreq, 41, 404, 255, 0);
  // Printing the RED (R) value
  Serial.print("GR = ");
  Serial.println(GredCol);
  delay(10);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(GS2,HIGH);
  digitalWrite(GS3,HIGH);
  // Reading the output frequency
  GgreenFreq = pulseIn(GSOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  GgreenCol = map(GgreenFreq, 47, 486, 255, 0);
  // Printing the GREEN (G) value  
  Serial.print(" GG = ");
  Serial.println(GgreenCol);
  delay(10);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(GS2,LOW);
  digitalWrite(GS3,HIGH);
  // Reading the output frequency
  GblueFreq = pulseIn(GSOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  GblueCol = map(GblueFreq, 42, 414, 255, 0);
  // Printing the BLUE (B) value 
  Serial.print(" GB = ");
  Serial.println(GblueCol);
  delay(10);
  

}
