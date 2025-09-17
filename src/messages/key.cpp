#include <Arduino.h>
#include "GlobalDefs.h"
#include "key.h"

// Declare or include the key image data as a uint16_t array
// extern const uint16_t keyImage[];

void drawKey(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    // tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, keyImage);
    delay(200);
}