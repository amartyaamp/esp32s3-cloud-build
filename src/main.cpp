#include <Arduino.h>
void setup() {
    Serial.begin(115200);
    Serial.println("Hello from Cloud Build!");
}
void loop() {
    Serial.println("Running...");
    delay(1000);
}
