#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 20);
  tft.println("Hello ESP32-S3 LCD 1.47!");
}

void loop() {
  tft.setCursor(10, 40);
  tft.println("It works!");
  delay(1000);
}
