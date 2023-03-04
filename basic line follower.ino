#define SLAVE_ADDRESS 0x04
#include <Wire.h>

int S1 = 35, S2 = 32, S3 = 34, S4 = 25, S5 = 26, S6 = 33;
int v1, v2, v3, v4, v5, v6;
int leftMotor_speed, rightMotor_speed, servoAngle;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
}

void loop() {
  v1 = analogRead(S1);
  v2 = analogRead(S2);
  v3 = analogRead(S3);
  v4 = analogRead(S4);
  v5 = analogRead(S5);
  v6 = analogRead(S6);
  
  Serial.print("sensor1:");
  Serial.print(v1);
  Serial.print(" sensor2:");
  Serial.print(v2);
  Serial.print(" sensor3:");
  Serial.print(v3);
  Serial.print(" sensor4:");
  Serial.print(v4);
  Serial.print(" sensor5:");
  Serial.print(v5);
  Serial.print(" sensor6:");
  Serial.println(v6);

  leftMotor_speed = rightMotor_speed = servoAngle = 0;

  if (v3 == 4095) {
    leftMotor_speed = 80;
    servoAngle = 92;
  }
  else if (v4 == 4095) {
    rightMotor_speed = 80;
    servoAngle = 92;
  }
  else {
    leftMotor_speed = rightMotor_speed = 120;
    servoAngle = 92;
  }

  transmitToArduino(leftMotor_speed, rightMotor_speed, servoAngle);
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



