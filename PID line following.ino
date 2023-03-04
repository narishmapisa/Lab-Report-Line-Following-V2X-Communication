#include <Wire.h> 

#include <PID_v1.h>  

#define I2C_SLAVE_ADDR  0x04 

#define S1 35
#define S2 32
#define S3 34
#define S4 25
#define S5 26
#define S6 33

int BaseSpeed = 80; 
int leftMotor_speed = 0; 
int rightMotor_speed = 0; 
int servoAngle = 40; 
int CentreAngle = 40; 

int v1,v2,v3,v4,v5,v6 = 0;
int Refpoint = 0; 
int WeightedAvg = 0; 
int error = 0; 
float K = 0; 
double ReferenceP, Input, U; 
double Kp=177, Ki=0.84, Kd=0; 

PID myPID(&Input, &U, &ReferenceP, Kp, Ki, Kd, DIRECT); //Define the PID object


void setup() {
  pinMode(S1, INPUT); 
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);

  Wire.begin(); 
  Serial.begin(9600); 
  myPID.SetMode(AUTOMATIC); 

}

void loop() {
  myPID.SetSampleTime(5); 
  Refpoint = 0; 


  v1 = map(constrain(analogRead(S1), 1200, 4095), 1200, 4095, 0, 254);
  v2 = map(constrain(analogRead(S2), 800, 4095), 800, 4095, 0, 254);
  v3 = map(constrain(analogRead(S3), 1500, 4095), 1500, 4095, 0, 254);
  v4 = map(constrain(analogRead(S4), 1600, 4095), 1600, 4095, 0, 254);
  v5 = map(constrain(analogRead(S5), 900, 4095), 900, 4095, 0, 254);
  v6 = map(constrain(analogRead(S6), 1300, 4095), 1300, 4095, 0, 254);



  v1 = 255 - v1;
  v2 = 255 - v2;
  v3 = 255 - v3;
  v4 = 255 - v4;
  v5 = 255 - v5;
  v6 = 255 - v6;


  // Calculate the weighted average of the Sensors 
  int numerator = (v1*1 + v2*2 + v3*3 + v4*3 + v5*2 + v6*1) / 12;
  float TotalWeight = (v1 + v2 + v3 + v4 + v5 + v6);

  float WeightedAvg = (numerator/TotalWeight);
  float error = ReferenceP - (WeightedAvg);
  Input = error;

  Serial.print("Weighted average: ");
  Serial.println(WeightedAvg);
  myPID.Compute();
  Serial.print("Output:");
  Serial.println(U);
 
  leftMotor_speed = BaseSpeed + (K*U);
  rightMotor_speed = BaseSpeed - (K*U);
  servoAngle = CentreAngle + U;
  
  leftMotor_speed = constrain(leftMotor_speed, 0, 255);
  rightMotor_speed = constrain(rightMotor_speed, 0, 255);
  servoAngle = constrain(servoAngle, -40, 160);
  Transmit_to_arduino(leftMotor_speed, rightMotor_speed, servoAngle); 

}

void Transmit_to_arduino(int leftMotor_speed, int rightMotor_speed, int servoAngle){
  Wire.beginTransmission(I2C_SLAVE_ADDR); 
  Wire.write((byte)((leftMotor_speed & 0x0000FF00) >> 8));    
  Wire.write((byte)(leftMotor_speed & 0x000000FF));          
  Wire.write((byte)((rightMotor_speed & 0x0000FF00) >> 8));  
  Wire.write((byte)(rightMotor_speed & 0x000000FF));          
  Wire.write((byte)((servoAngle & 0x0000FF00) >> 8));         
  Wire.write((byte)(servoAngle & 0x000000FF)); 
  Wire.endTransmission();   // stop transmitting
}
