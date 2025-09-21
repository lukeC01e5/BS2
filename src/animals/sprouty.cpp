#include <Arduino.h>
#include "GlobalDefs.h"
#include "sprouty.h"

void drawSprouty(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    // tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, sprouty);
    delay(2000);
}