#include <Arduino.h>  // ✅ Required for setup() and loop()

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, World from ESP32-S3!");
}

void loop() {
  // Nothing here
}
