#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>

// Network Information
const char* ssid = "YOUR_NETWORK_NAME";
const char* password = "YOUR_NETWORK_PASSWORD";

const int stepsPerRevolution = 2048; // Number of steps per revolution
const int RevolutionsPerMinute = 5;  // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
const unsigned long coolDown = 5000; // How long before the door relocks automatically
bool unlocked = false;
unsigned long currentTime = millis();

// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

// Initialize the stepper library on pins
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  // Connect to home network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
Serial.println("WiFi connected!");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

  // Set Speed
  myStepper.setSpeed(RevolutionsPerMinute);
}

void loop() {
  char incoming = Serial.read();

  if (incoming == 'u' && !unlocked) {
    myStepper.step(stepsPerRevolution);
    unlocked = true;
    Serial.println("Unlocked");
    currentTime = millis();
  }

  if (incoming == 'l' && unlocked) {
    myStepper.step(-stepsPerRevolution);
    unlocked = false;
    Serial.println("Locked");
  }

  if (unlocked && (millis() - currentTime > coolDown)) {
    myStepper.step(-stepsPerRevolution);
    unlocked = false;
    Serial.println("Locked");
  }
}