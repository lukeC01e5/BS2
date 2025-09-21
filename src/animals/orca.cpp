#include <Arduino.h>
#include "GlobalDefs.h"

#include "squidy.h"
#include "orca.h" // Assuming you have an orca.h file similar to squidy.h



void drawOrca(TFT_eSPI &tft)
{
    // Fill the background:
    tft.fillScreen(TFT_BLACK);

    tft.setCursor(0, 0);

    tft.setSwapBytes(true);

    // Example: push an image from array data
    int x = 0;
    int y = 0;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, orca); // If 'squidy' array is defined
    delay(2000);                                // Show for 2 seconds
}