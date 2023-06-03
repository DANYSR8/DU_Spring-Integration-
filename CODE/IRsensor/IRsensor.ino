int IRSensor = 4; // connect IR sensor module to Teensy pin 2
#include <Servo.h>
int PWMSignal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Init Serial at 115200 Baud Rate.
  Serial.println("Serial Working"); // Test to check if serial is working or not
  pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
}

void loop() {

  //IR sensor
  // put your main code here, to run repeatedly:
  int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input
  delay(1000);
  if (sensorStatus == 1) // Check if the pin high or not
  {
    Serial.println("No Object Detected"); // print Motion Detected! on the serial monitor window
  }
  else  {
    Serial.println("Object detected"); // print Motion Ended! on the serial monitor window
  }
  
  delay(500);

}

//base code from https://circuitdigest.com/microcontroller-projects/interfacing-ir-sensor-module-with-arduino

