// Library for the LCD display
#include <LiquidCrystal.h>

// Libraries for the nRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Defining the nRF24L01 pins
// It also uses pins 11, 12, 13 for SPI
RF24 radio(A3, A4); // CE, CSN

// The address with which the nRF24L01s recognizes the other
const byte address[6] = "TIVMF";

// Pins used by the motor driver
int enA = 3;
int in1 = A1;
int in2 = A2;

// Pin used by the LCD shield keypad
const int BUTTONS = A0;

// Pins used by the shield
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_RS = 8;
const int pin_EN = 9;

// Initializing the LCD keypad shield
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Parameter for the choice of the user
int choose = 0;

// The height at which the motor will stop (in cm)
int stopHeight = 5;

// Function to print the starting menu
void start() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Option:");

  lcd.setCursor(9, 0);
  lcd.print("Fast");

  lcd.setCursor(9, 1);
  lcd.print("Slow");
}

// Function to indicate the selected choice by arrow
void setArrow() {
  start();

  if (choose == 1) {
    lcd.setCursor(8, 0);
    lcd.print(">");
  } 
  
  else if (choose == 2) {
    lcd.setCursor(8, 1);
    lcd.print(">");
  }
}

// Function to move the selector - the arrow
// It executes the selected mode after clicking the select button
void selectMode() {
  start();

  int lcdButtons;

  while (true) {
    lcdButtons = analogRead(BUTTONS);
    delay(75);

    if (lcdButtons > 60 && lcdButtons < 200) {
      choose = 1;
      setArrow();
    } 
    
    else if (lcdButtons > 200 && lcdButtons < 400) {
      choose = 2;
      setArrow();
    } 
    
    else if (lcdButtons > 600 && lcdButtons < 800) {
      if(choose != 0){
        startSpinning();
        return;
      }

      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please select");
        lcd.setCursor(0, 1);
        lcd.print("mode first!");

        delay(3000);

        setArrow();
      }
    }
  }
}

// Function to execute the choice of the user
void startSpinning() {
  lcd.clear();
  lcd.setCursor(0, 0);

  String speedText;
  unsigned long timeSpam;

  // Display the selected option
  if (choose == 1) {
    // Setting the motor at fast speed
    analogWrite(enA, 200);
    speedText = "Fast";
    timeSpam = 600000;
  }
  
  if (choose == 2) {
    // Setting the motor at slow speed
    analogWrite(enA, 100);
    speedText = "Slow";
    timeSpam = 1200000;
  }

  lcd.print("Option: ");
  lcd.setCursor(10, 0);
  lcd.print(speedText);

  lcd.setCursor(0, 1);
  lcd.print("Remaining:");

  // Starting the action of the motor
  // If you swap the height and low, you'll change the direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Start the timer for 20 minutes
  unsigned long timeStarted = millis();

  while (millis() - timeStarted < timeSpam) {
    // Looking for the message if the honey level is too high
    if (radio.available()) {
      int honeyHeight = 0;
      radio.read(&honeyHeight, sizeof(honeyHeight));

      // Seeing if the honey level is too high
      if (honeyHeight <= stopHeight) {
        // Stopping the motor
        analogWrite(enA, 0);

        // Alerting the user for the honey level
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Process stopped!");
        lcd.setCursor(0, 1);
        lcd.print("Resume -> select");

        // For user input
        int lcdButtons = 0;

        // Waiting for the user to resume by clicking select
        while (lcdButtons < 600 || lcdButtons > 800) {
          lcdButtons = analogRead(BUTTONS);
        }

        // Resuming the motor action
        lcd.clear();
        lcd.print("Option: ");
        lcd.setCursor(10, 0);
        lcd.print(speedText);

        if (choose == 1) {
          // Setting the motor at fast speed
          analogWrite(enA, 200);
        } 
        
        else if (choose == 2) {
          // Setting the motor at slow speed
          analogWrite(enA, 100);
        }

        lcd.setCursor(0, 1);
        lcd.print("Remaining:");

        radio.flush_rx();

        honeyHeight = 0;
      }
    }

    unsigned long minutes = (timeSpam - (millis() - timeStarted)) / 60000;
    unsigned long seconds = ((timeSpam - (millis() - timeStarted)) / 1000) % 60;

    lcd.setCursor(11, 1);
    lcd.print(minutes);
    lcd.print(":");

    if (seconds < 10) {
      lcd.print("0");
    }
    lcd.print(seconds);
    lcd.print(" ");
  }

  // Message when the time is over
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Time's up!");
  delay(2000);

  // Stopping the motor
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  radio.flush_tx();

  choose = 0;
}

void setup() {
  // Starting the LCD shield
  lcd.begin(16, 2);

  // Setting up the motor pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  // Turn off motor - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Starting the nRF24L01
  radio.begin();

  // Opening a channel to read from
  radio.openReadingPipe(1, address);

  // Setting the power level - distance in which the nRF24L01s work
  radio.setPALevel(RF24_PA_MIN);

  // Preparing the nRF24L01 to listen
  radio.startListening();
}

void loop() {
  selectMode();
}
