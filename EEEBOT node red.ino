
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt;


// Replace the next variables with your SSID/Password combination
const char* ssid = "b26wifi";
const char* password = "b26password";                

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup() {
  Serial.begin(115200);
Wire.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  mySensor.setWire(&Wire);
  mySensor.beginAccel();

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");

    }
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    mySensor.accelUpdate();
    publishdata("esp32/3/ax",mySensor.accelX());
    publishdata("esp32/3/ay",mySensor.accelY());
    publishdata("esp32/3/az",mySensor.accelZ());
    aSqrt = mySensor.accelSqrt();


  
  
  }
}

void publishdata(char *topic, float value)
{
    char tempString[10];
    dtostrf(value, 3, 2, tempString);
    Serial.print(" topic: ");
    Serial.println(topic);
    Serial.print(" value: ");
    Serial.println(value);
    client.publish(topic, tempString);

}
