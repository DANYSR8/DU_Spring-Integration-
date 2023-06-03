#define Buzzer 13

void setup() {
  pinMode(Buzzer, OUTPUT); 


}

void loop() {
  
  digitalWrite(Buzzer, HIGH); 
  delay(1000);
  digitalWrite(Buzzer, LOW); 
  delay(1000);



}
