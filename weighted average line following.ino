#define SLAVE_ADDRESS 0x04
#include <Wire.h>

// Define pins for each sensor
int S1 = 35, S2 = 32, S3 = 34, S4 = 25, S5 = 26, S6 = 33;

// Define variables to hold sensor readings and motor speeds
int v1, v2, v3, v4, v5, v6;
int leftMotor_speed, rightMotor_speed, servoAngle;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Set pin modes for each sensor
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
}
void loop() {
  // Read sensor values
  v1 = analogRead(S1);
  v2 = analogRead(S2);
  v3 = analogRead(S3);
  v4 = analogRead(S4);
  v5 = analogRead(S5);
  v6 = analogRead(S6);
  
  // Compute the weighted average using all six sensors
  int weightedAverage = (v1*1 + v2*2 + v3*3 + v4*3 + v5*2 + v6*1) / 12;
  
  // Print sensor values and weighted average to the serial monitor
  Serial.print("Sensor1: ");
  Serial.print(v1);
  Serial.print(", Sensor2: ");
  Serial.print(v2);
  Serial.print(", Sensor3: ");
  Serial.print(v3);
  Serial.print(", Sensor4: ");
  Serial.print(v4);
  Serial.print(", Sensor5: ");
  Serial.print(v5);
  Serial.print(", Sensor6: ");
  Serial.print(v6);
  Serial.print(", Weighted Average: ");
  Serial.println(weightedAverage);

  // Adjust the motor speeds and servo angle based on the weighted average
  if (weightedAverage >= 2500) {
    // Robot is on black line, so drive straight
    leftMotor_speed = rightMotor_speed = 120;
    servoAngle = 92;
  }
  else if (weightedAverage < 2500 && weightedAverage >= 2000) {
    // Robot is slightly off the line
    if (v1 + v2 < v5 + v6) {
      // Turn slightly to the right
      leftMotor_speed = 100;
      rightMotor_speed = 140;
      servoAngle = 92;
    } else {
      // Turn slightly to the left
      leftMotor_speed = 140;
      rightMotor_speed = 100;
      servoAngle = 92;
    }
  }
  else {
    // Robot is significantly off the line
    if (v1 + v2 < v5 + v6) {
      // Turn sharply to the right
      leftMotor_speed = 80;
      rightMotor_speed = 160;
      servoAngle = 92;
    } else {
      // Turn sharply to the left
      leftMotor_speed = 160;
      rightMotor_speed = 80;
      servoAngle = 92;
    }
  }

  void transmitToArduino(int leftMotor_speed, int rightMotor_speed, int servoAngle) {
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write((byte)((leftMotor_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(leftMotor_speed & 0x000000FF));
  Wire.write((byte)((rightMotor_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(rightMotor_speed & 0x000000FF));
  Wire.write((byte)((servoAngle & 0x0000FF00) >> 8));
  Wire.write((byte)(servoAngle & 0x000000FF));
  Wire.endTransmission();
}



