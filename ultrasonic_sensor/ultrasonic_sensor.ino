// Libraries for the nRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Defining the nRF24L01 pins
// It also uses pins 11, 12, 13 for SPI
RF24 radio(7, 8); // CE, CSN

// The address with which the nRF24L01s recognises the other
const byte address[6] = "TIVMF";

// Defining the ultrasonic sensor pins
const int triggerPin = 2;
const int echoPin = 3;

// Hight at which the sensor trigers
int triggerHight = 4;

void setup() {
  // Starting the nRF24L01
  radio.begin();

  // Defining a chanel to send info to
  radio.openWritingPipe(address);

  // Setting the power level - distance in which the nRF24L01s work
  radio.setPALevel(RF24_PA_MIN);

  // nRF24L01 stops listening and prepares to write
  radio.stopListening();
}

long microsecondsToCentimeters(long microseconds){
  // The speed of sound = 340 m/s or 29 microseconds per centimeter
  return microseconds / 29;
}

void loop(){
  long duration;
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

  // When water level is close eneogh
  // a message is passed to the main part
  if(cm <= triggerHight){
    const char text[] = "Hello World";
    radio.write(&text, sizeof(text));
  }

  delay(1000);
}
