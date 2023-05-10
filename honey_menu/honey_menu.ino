#include <LiquidCrystal.h>

// Pins used by the motor driver
int motorPin1 = 2;
int motorPin2 = 3;

// Pin to control the speed of the motor
int motorSpeed = 11;

// Pin used by the lcd shield keypad
const int BUTTONS = 0;

// Pins used by the shield
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_RS = 8;
const int pin_EN = 9;

// Initializing the lcd keypad shield
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Parameter for the choice of the user
int choose = 1;

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
  if(choose == 1) {
    start();

    lcd.setCursor(8, 0);
    lcd.print(">");
  }

  else if(choose == 2) {
    start();

    lcd.setCursor(8, 1);
    lcd.print(">");
  }
}

// Function to move the selector - the arrow
// It executes the selected mode after clicking the select button
void selectMode() {
  start();

  int lcdButtons;

  while(1){
    lcdButtons = analogRead(BUTTONS);
    delay(75);

    if(lcdButtons > 60 && lcdButtons < 200) {
      choose = 1;
      setArrow();
    }

    if(lcdButtons > 200 && lcdButtons < 400) {
      choose = 2;
      setArrow();
    }
    
    if(lcdButtons > 600 && lcdButtons < 800) {
      startSpinning();
      return;
    }
  }
}

// Function to execute the choice of the user
void startSpinning() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // Display the selected option
  if (choose == 1) {
    lcd.print("Option: Fast");

    // Setting the motor at fast speed
    analogWrite(motorSpeed, 250);
  } 
  
  else if (choose == 2) {
    lcd.print("Option: Slow");

    // Setting the motor at slow speed
    analogWrite(motorSpeed, 190);
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Remaining:");

  // Starting the action of the motor
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  
  // Start the timer for 20 minutes
  int secRemaining = 20 * 60;

  while (secRemaining > 0) {
    int minutes = secRemaining / 60;
    int seconds = secRemaining % 60;
    
    lcd.setCursor(11, 1);
    lcd.print(minutes);
    lcd.print(":");
    
    if (seconds < 10) {
      lcd.print("0");
    }
    lcd.print(seconds);
    
    delay(1000);
    secRemaining--;
  }
  
  // Mesage when the time is over
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Time's up!");
  delay(2000);

  // Stoping the motor
  analogWrite(motorSpeed, 0);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  
  pinMode(motorSpeed, OUTPUT);
}

void loop() {
  selectMode();
}
