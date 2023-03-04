// Define the pins
const int pin35 = 35;
const int pin32 = 32;
const int pin34 = 34;
const int pin25 = 25;
const int pin26 = 26;
const int pin33 = 33;

void setup() {
  Serial.begin(9600); // Start the serial communication
  pinMode(pin34, INPUT); // Set the pins as inputs
  pinMode(pin35, INPUT);
  pinMode(pin32, INPUT);
  pinMode(pin33, INPUT);
  pinMode(pin25, INPUT);
  pinMode(pin26, INPUT);
}

void loop() {
  // Read the values from the pins
  int val34 = analogRead(pin34);
  int val35 = analogRead(pin35);
  int val32 = analogRead(pin32);
  int val33 = analogRead(pin33);
  int val25 = analogRead(pin25);
  int val26 = analogRead(pin26);

  // Print the values to the serial monitor, left to right when array is facing down

  Serial.print("S1:");
  Serial.print(val35);
  Serial.print(" S2:");
  Serial.print(val32);
  Serial.print(" S3:");
  Serial.print(val34);
  Serial.print(" S4:");
  Serial.print(val25);
  Serial.print(" S5:");
  Serial.print(val26);
  Serial.print(" S6:");
  Serial.print(val33);
 Serial.print("\n");
 

  delay(1000); // Wait for 2 seconds before repeating
}

