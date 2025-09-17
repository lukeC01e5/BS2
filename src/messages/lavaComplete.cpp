#include <Arduino.h>
#include "GlobalDefs.h"
#include "lavaComplete.h"

// Declare or include the lava complete image data as a uint16_t array

void drawLavaComplete(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    // FIX: Match the array size (16200 = 135×120)
    int width = 120; // Changed from 120
    int height = 135; // Changed from 128

    tft.pushImage(x, y, width, height, lavaComplete);
    delay(200);
}