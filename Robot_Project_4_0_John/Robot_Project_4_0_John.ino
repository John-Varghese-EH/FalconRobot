/*
Project Name: Smart AI Waste Management Robot (Falcon 4.0.3 UNO - BT Controller)
Base: Falcon 4.0.3 optimized + BT Controller compatibility
Author: John Varghese (Modified by Assistant)
Author Instagram: @cyber__trinity @john__varghese @trendy_ned
Author Channel URI: https://youtube.com/@trendy_ned
App: BT Controller, WebServer(ESP32-CAM)
Description:
  Arduino UNO-compatible robot controlled via BT Controller app (HC-05),
  with obstacle avoidance using ultrasonic sensors (front & back).
  Works with Adafruit AFMotor Shield and supports buzzer and LED.
Version: 4.0.3
License: Remixing or Changing this Thing is not allowed. Commercial use is not allowed.
This Program is created for Exbition/Display purpose only. The all credits are to the author "John Varghese"
*/

#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// ==================== EMBEDDED ATTRIBUTION =========================
const char ATTRIBUTION[] PROGMEM = 
  "Falcon Robot v4.0.3 | "
  "Author: John Varghese | "
  "Instagram: @cyber__trinity @john__varghese | "
  "YouTube: youtube.com/@trendy_ned | "
  "License: All Rights Reserved - No Commercial Use | "
  "Created: 2026";
// ===================================================================

// Motor definitions
AF_DCMotor MotorFR(1);   // Front right
AF_DCMotor MotorFL(2);   // Front left
AF_DCMotor MotorBL(3);   // Back left
AF_DCMotor MotorBR(4);   // Back right

// Servo definitions
Servo servo_1;
Servo servo_2;
int angle = 90;
int angle2 = 90;
int angleStep = 5;
bool controlMode = 0; // 0: Car, 1: Servo

// Ultrasonic pins
#define front_trig_pin A3
#define front_echo_pin A2
#define back_trig_pin  A1
#define back_echo_pin  A0
#define maximum_distance 200

NewPing front_sensor(front_trig_pin, front_echo_pin, maximum_distance);
NewPing back_sensor(back_trig_pin, back_echo_pin, maximum_distance);

// IO pins
const int buzPin = 2;     // buzzer
const int ledPin = A5;    // headlight LED
const int statusLed = 13;  // ultrasonic indicator LED
const int binSensor = A4;  // IR Bin Full Sensor

// State variables
int valSpeed = 255;
int front_distance = 100;
int back_distance = 100;
int UbSr = 1;
char command;
bool binIsFull = false;

// Function Prototypes for smooth movement
void serUp();
void serDown();
void slowInitialMove();
void checkBinStatus();

void setup() {
  Serial.begin(9600);
  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(statusLed, OUTPUT);
  pinMode(binSensor, INPUT);

  stopAll();

  MotorFL.setSpeed(valSpeed);
  MotorFR.setSpeed(valSpeed);
  MotorBL.setSpeed(valSpeed);
  MotorBR.setSpeed(valSpeed);

  // Servo setup
  servo_1.attach(9);
  servo_2.attach(10);
  slowInitialMove(); // Move to default 90,90 slowly

  Serial.println("Falcon 4.0 - BT Ready");
}

void loop() {
  if (Serial.available()) {
    command = Serial.read();
    handleCommand(command);
  }
  checkBinStatus();
  if (!binIsFull) {
    obstacleCheck();
  }
}

// Handle Bluetooth commands
void handleCommand(char cmd) {
  switch (cmd) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'G': forwardLeft(); break;
    case 'H': forwardRight(); break;
    case 'I': backLeft(); break;
    case 'J': backRight(); break;
    case 'S': stopAll(); break;
    case 'Y': honk(); break;
    case 'U': digitalWrite(ledPin, HIGH); break;
    case 'u': digitalWrite(ledPin, LOW); break;
    case '1': setSpeed(65); break;
    case '2': setSpeed(130); break;
    case '3': setSpeed(195); break;
    case '4': setSpeed(255); break;
    case 'X': 
      controlMode = !controlMode; 
      stopAll();
      Serial.print("Mode: ");
      Serial.println(controlMode ? "Servo" : "Car");
      break;
  }

  // Handle movements based on mode
  if (cmd == 'F') {
    if (controlMode) serUp(); else moveForward();
  } else if (cmd == 'B') {
    if (controlMode) serDown(); else moveBackward();
  } else if (cmd == 'L' && !controlMode) {
    turnLeft();
  } else if (cmd == 'R' && !controlMode) {
    turnRight();
  } else if (cmd == 'S') {
    stopAll();
  }
}

// Motor speed controller
void setSpeed(int speedVal) {
  valSpeed = speedVal;
  MotorFL.setSpeed(valSpeed);
  MotorFR.setSpeed(valSpeed);
  MotorBL.setSpeed(valSpeed);
  MotorBR.setSpeed(valSpeed);
}

// Movement functions
void moveForward() {
  setSpeed(valSpeed);
  MotorFL.run(FORWARD); MotorFR.run(FORWARD);
  MotorBL.run(FORWARD); MotorBR.run(FORWARD);
}
void moveBackward() {
  setSpeed(valSpeed);
  MotorFL.run(BACKWARD); MotorFR.run(BACKWARD);
  MotorBL.run(BACKWARD); MotorBR.run(BACKWARD);
}
void turnLeft() {
  setSpeed(valSpeed);
  MotorFL.run(BACKWARD); MotorFR.run(FORWARD);
  MotorBL.run(BACKWARD); MotorBR.run(FORWARD);
}
void turnRight() {
  setSpeed(valSpeed);
  MotorFL.run(FORWARD); MotorFR.run(BACKWARD);
  MotorBL.run(FORWARD); MotorBR.run(BACKWARD);
}
void forwardLeft() {
  MotorFL.setSpeed(valSpeed / 3);
  MotorBL.setSpeed(valSpeed / 3);
  MotorFL.run(FORWARD); MotorFR.run(FORWARD);
  MotorBL.run(FORWARD); MotorBR.run(FORWARD);
}
void forwardRight() {
  MotorFR.setSpeed(valSpeed / 3);
  MotorBR.setSpeed(valSpeed / 3);
  MotorFL.run(FORWARD); MotorFR.run(FORWARD);
  MotorBL.run(FORWARD); MotorBR.run(FORWARD);
}
void backLeft() {
  MotorFL.setSpeed(valSpeed / 3);
  MotorBL.setSpeed(valSpeed / 3);
  MotorFL.run(BACKWARD); MotorFR.run(BACKWARD);
  MotorBL.run(BACKWARD); MotorBR.run(BACKWARD);
}
void backRight() {
  MotorFR.setSpeed(valSpeed / 3);
  MotorBR.setSpeed(valSpeed / 3);
  MotorFL.run(BACKWARD); MotorFR.run(BACKWARD);
  MotorBL.run(BACKWARD); MotorBR.run(BACKWARD);
}
void stopAll() {
  MotorFL.run(RELEASE); MotorFR.run(RELEASE);
  MotorBL.run(RELEASE); MotorBR.run(RELEASE);
}

// Horn/Buzzer
void honk() {
  digitalWrite(buzPin, HIGH);
  delay(200);
  digitalWrite(buzPin, LOW);
  delay(80);
  digitalWrite(buzPin, HIGH);
  delay(300);
  digitalWrite(buzPin, LOW);
}

// Ultrasonic obstacle detection
void obstacleCheck() {
  front_distance = readFrontPing();
  back_distance  = readBackPing();

  // Obstacle in front
  if (front_distance <= 20) {
    stopAll();
    digitalWrite(statusLed, HIGH); // Turn on D13 indicator
    digitalWrite(ledPin, HIGH);
    honk();
    delay(200);
    moveBackward();
    delay(300);
    stopAll();
    delay(200);
    digitalWrite(ledPin, LOW);
    digitalWrite(statusLed, LOW); // Turn off D13 indicator
  }

  // Obstacle in back
  if (back_distance <= 20 && UbSr == 1) {
    stopAll();
    digitalWrite(statusLed, HIGH); // Turn on D13 indicator
    digitalWrite(ledPin, HIGH);
    honk();
    delay(200);
    moveForward();
    delay(300);
    stopAll();
    delay(200);
    digitalWrite(ledPin, LOW);
    digitalWrite(statusLed, LOW); // Turn off D13 indicator
  }
}

// Ultrasonic reading functions
int readFrontPing() {
  int cm = front_sensor.ping_cm();
  return (cm == 0) ? 250 : cm;
}
int readBackPing() {
  int cm = back_sensor.ping_cm();
  return (cm == 0) ? 250 : cm;
}

// --- Servo Logic ---

void serUp() {
  // Move servo 1 down, servo 2 up (Logic from v2.0.2)
  if (angle > 0) {
    angle -= angleStep;
    if (angle < 0) angle = 0;
    servo_1.write(angle);
  }
  
  if (angle2 < 180) {
    angle2 += angleStep;
    if (angle2 > 180) angle2 = 180;
    servo_2.write(angle2);
  }

  // Blocking logic for ultrasonic sensor
  if ((angle <= 125 && angle >= 55) || (angle2 >= 55 && angle2 <= 125)) {
    UbSr = 0;
  } else {
    UbSr = 1;
  }
  
  delay(50); // Small delay to prevent sudden current draw
}

void serDown() {
  // Move servo 1 up, servo 2 down (Logic from v2.0.2)
  if (angle < 180) {
    angle += angleStep;
    if (angle > 180) angle = 180;
    servo_1.write(angle);
  }
  
  if (angle2 > 0) {
    angle2 -= angleStep;
    if (angle2 < 0) angle2 = 0;
    servo_2.write(angle2);
  }

  // Blocking logic for ultrasonic sensor
  if ((angle <= 125 && angle >= 55) || (angle2 >= 55 && angle2 <= 125)) {
    UbSr = 0;
  } else {
    UbSr = 1;
  }
  
  delay(50); // Small delay to prevent sudden current draw
}

void slowInitialMove() {
  // We don't know the exact current position, so we snap to 90 once
  // then ensure any subsequent moves are controlled.
  // To strictly "move slowly to initial", we'd need to know where it is.
  // Here we just sweep from 0 to 90 to give a "slow" power-on feel.
  for(int i = 0; i <= 90; i++) {
    servo_1.write(i);
    servo_2.write(180-i); // Opposite move for the pair
    delay(30);
  }
  angle = 90;
  angle2 = 90;
}

void checkBinStatus() {
  // Most IR modules go LOW when an object is detected (Bin Full)
  if (digitalRead(binSensor) == LOW) {
    if (!binIsFull) { // Just triggered
      binIsFull = true;
      stopAll();
      Serial.println("ALERT: BIN FULL!");
      honk();
    }
    // Visual alert: Rapid flash status LED
    digitalWrite(statusLed, (millis() / 200) % 2); 
  } else {
    if (binIsFull) { // Just emptied
      binIsFull = false;
      digitalWrite(statusLed, LOW);
      Serial.println("Bin Emptied - Ready");
    }
  }
}
