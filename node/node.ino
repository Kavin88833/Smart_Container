//Node MCU
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "https://smartcontainer-fcd6f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyC3A17TtSqJL7bFDRCYcjO_1MnWd7otbT4"

#define WIFI_SSID "g8x"
#define WIFI_PASSWORD "123456789"

const int inputPin = D3;

int firebase_id;

String h;
String w;
String bmi;

FirebaseData firebaseData;

//FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;


void setup() {
  Serial.begin(9600);
  Serial.println("Node started");
  pinMode(inputPin, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("v3");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
}

void loop() {
  int pinState = digitalRead(inputPin);
  if (pinState == HIGH) {
    //Serial.println("High detected");
    if (Serial.available()) { 
        String data = Serial.readStringUntil('\n');
        Serial.println("Node Received after: " + data);
        data.replace("\n", "");  // Remove new line characters
        data.replace("\r", "");  // Remove carriage return characters
        //data.trim();             // Trim whitespace from both ends
        Serial.println("Node Received after: " + data);
        int idx = data.indexOf("/");
        String a = data.substring(0,idx);
        String b = data.substring(idx+1);
        //Serial.println(a+"  "+b);
        // Send data to Firebase
        if (Firebase.setString(firebaseData,"/items/"+a, b)) {
          Serial.println("Data sent to Firebase successfully");
          Serial.flush();
          delay(500);
        } else {
          Serial.println("Failed to send data. Error: " + firebaseData.errorReason());
        }
      }
    } else {
      Serial.println("Arduino Mega is LOW");
    }
    delay(1000);
  }