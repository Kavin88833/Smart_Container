const int inputPin = 50; // Arduino Mega pin 7
const int outputPin = 51;


void setup() {
  Serial.begin(19200);
  Serial1.begin(9600); 
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  //int pinState = digitalRead(inputPin);
  digitalWrite(outputPin, !digitalRead(outputPin)); // Toggle pin state
  delay(2000); // Delay for 2 seconds
  Serial.println("Message sent: ");
  Serial1.println("Hello NodeMCU");  
  /*if (pinState == HIGH) {
    Serial.println("Arduino Mega: NodeMCU Pin D2 is HIGH");
  } else {
    Serial.println("Arduino Mega: NodeMCU Pin D2 is LOW");
  }
  delay(1000); // Delay for 1 second before next reading */
}
