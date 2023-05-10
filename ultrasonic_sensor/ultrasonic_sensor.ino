// Defining the ultrasonic sensor pins
const int triggerPin = 2;
const int echoPin = 3;

// Hight at withc the sensor trigers
int triggerHight = 2;

void setup(){
  Serial.begin(9600);
}

long microsecondsToCentimeters(long microseconds){
  // The speed of sound = 340 m/s or 29 microseconds per centimeter
  return microseconds / 29;
}

void loop(){
  long duration;
  long inches;
  long cm;

  // Next section sends signals in every 10 microsecond
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Catching the returning value
  // It is in mocroseconds 
  // The time it travalled to the object and then returned
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Traversing the duration into cm
  // It is after that devided by two,
  // because the distace = traveling to + going back
  cm = microsecondsToCentimeters(duration) / 2;

  if(cm <= 4){
    Serial.print(cm);
  }
  Serial.print("\n");

  delay(1000);
}