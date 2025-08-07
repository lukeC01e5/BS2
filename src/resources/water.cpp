
#include <Arduino.h>
#include "GlobalDefs.h"

#include "water.h"



void drawWater(TFT_eSPI &tft)
{
    // Fill the background:
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setSwapBytes(true);

    // Example: push an image from array data
    int x = 20;
    int y = 8;
    int width = 190;
    int height = 128;
    tft.pushImage(x, y, width, height, water); // If 'squidy' array is defined
    delay(1000);                                // Show for 2 seconds
}