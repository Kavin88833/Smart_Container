#include <ESP8266WiFi.h>

const int outputPin = D2; // NodeMCU pin D2 (GPIO 4)
const int inputPin = D3;

void setup() {
  Serial.begin(9600);
  pinMode(outputPin, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(outputPin, LOW); // Initial state LOW
  Serial.println("NodeMCU: Pin D2 blinking...");
}

void loop() {
  //digitalWrite(outputPin, !digitalRead(outputPin)); // Toggle pin state
  //delay(2000); // Delay for 2 seconds
  int pinState = digitalRead(inputPin);
  if (pinState == HIGH) {
    if (Serial.available()) {           // Check if there is incoming data
    String data = Serial.readString(); // Read the incoming data as string
    Serial.println("Node Received:" + data);}
  } else {
    Serial.println("Arduino Mega is LOW");
  }
  delay(1000);
}
