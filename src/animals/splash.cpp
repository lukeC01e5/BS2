#include <Arduino.h>
#include "GlobalDefs.h"
#include "splash.h"

void drawSplash(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    // tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, splash);
    delay(2000);
}