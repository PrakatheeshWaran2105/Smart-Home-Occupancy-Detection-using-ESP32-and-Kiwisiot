/*
  Project: Smart Home Occupancy Detection
  Board: NodeMCU ESP8266
  Sensor: HC-SR501 PIR
  Platform: KiwisIoT
*/

#include <ESP8266WiFi.h>
#include <KiwisIoT.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* pass = "YOUR_WIFI_PASSWORD";

// KiwisIoT dashboard Topic ID
const char* topic = "YOUR_KIWISIOT_TOPIC_ID";

// PIR sensor pin
#define PIR_PIN D5

// KiwisIoT connection
KiwisIoT kiwisiot(ssid, pass, topic);

int previousMotionState = -1;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  Serial.println();
  Serial.println("Smart Home Occupancy Detection");
  Serial.println("Initializing PIR sensor...");

  kiwisiot.begin();

  // Allow PIR sensor to stabilize
  delay(30000);

  Serial.println("System Ready");
}

void loop() {

  // Maintain KiwisIoT connection
  kiwisiot.run();

  // Read PIR sensor
  int motionState = digitalRead(PIR_PIN);

  // Display status when it changes
  if (motionState != previousMotionState) {

    previousMotionState = motionState;

    if (motionState == HIGH) {
      Serial.println("Motion Detected!");
    } else {
      Serial.println("No Motion");
    }
  }

  // Send status to KiwisIoT every 2 seconds
  if (millis() - lastSendTime >= sendInterval) {

    lastSendTime = millis();

    String statusValue = String(motionState);

    // Channel 0: 1 = Motion, 0 = No Motion
    kiwisiot.send("0", statusValue);

    Serial.print("KiwisIoT Status Sent: ");
    Serial.println(statusValue);
  }

  delay(10);
}