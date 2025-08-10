#include <Arduino.h>
#include <TFT_eSPI.h> // Uses User_Setup.h in library

TFT_eSPI tft = TFT_eSPI(); 

void setup() {
  tft.init();
  tft.setRotation(1);  // adjust for your screen orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Hello World");
}

void loop() {
  // Nothing needed here for static text
}
