#define echoPin 41 // attach Echo of HC-SR04
#define trigPin 40//attach Trig of HC-SR04
long duration;
int distance;

void setup() {
  Serial.begin(115200);//  setup serial
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (send & receive)

  Serial.print(distance);
  Serial.println("cm");
  delay(100);
}
