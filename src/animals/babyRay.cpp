#include <Arduino.h>
#include "GlobalDefs.h"

#include "babyRay.h"

void drawBabyRay(TFT_eSPI &tft)
{
    // Fill the background:
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);

    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, babyRay); // If 'squidy' array is defined
    delay(2000);                                   // Show for 2 seconds
}