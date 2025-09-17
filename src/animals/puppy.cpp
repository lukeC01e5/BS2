#include <Arduino.h>
#include "GlobalDefs.h"
#include "puppy.h"

void drawPuppy(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    // tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 120;
    int height = 128;
    tft.pushImage(x, y, width, height, puppy);
    delay(2000);
}