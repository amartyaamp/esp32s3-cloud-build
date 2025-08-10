#define ST7789_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// Waveshare ESP32-S3 LCD 1.47" pins
#define TFT_MISO -1
#define TFT_MOSI 35
#define TFT_SCLK 36
#define TFT_CS   34
#define TFT_DC   37
#define TFT_RST  38
#define TFT_BL   45

#define TFT_BACKLIGHT_ON HIGH

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define SMOOTH_FONT
