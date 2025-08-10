#include <Arduino.h>
#include <TFT_eSPI.h>  // Uses User_Setup_Select.h

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Hello World", 10, 10, 4);
}

void loop() {
  delay(1000);
}
