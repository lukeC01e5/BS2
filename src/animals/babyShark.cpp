#include <Arduino.h>
#include "GlobalDefs.h"

#include "babyShark.h"

void drawBabyShark(TFT_eSPI &tft)
{
    // Fill the background:
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setSwapBytes(true);

    // Example: push an image from array data
    int x = 0;
    int y = 8;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, babyShark); // If 'squidy' array is defined
    delay(2000);                                   // Show for 2 seconds
}