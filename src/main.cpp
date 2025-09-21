#include <Arduino.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // number of steps per revolution
const int RevolutionsPerMinute = 17;  // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
const int coolDown = 5000;            // how long before the door relocks automatically
int unlocked = 0;
int currentTime = millis();

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(RevolutionsPerMinute);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  char incoming = Serial.read();

  if (incoming == 'u' && !unlocked) {
    myStepper.step(stepsPerRevolution);
    unlocked = 1;
    Serial.println("Unlocked");
    currentTime = millis();
  }

  if (incoming == 'l' && unlocked) {
    myStepper.step(-stepsPerRevolution);
    unlocked = 0;
    Serial.println("Locked");
  }

  if (unlocked && (millis() - currentTime > coolDown)) {
    myStepper.step(-stepsPerRevolution);
    unlocked = 0;
    Serial.println("Locked");
  }
}