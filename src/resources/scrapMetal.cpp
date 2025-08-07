#include <Arduino.h>
#include "GlobalDefs.h"
#include "scrapMetal.h"

void drawScrapMetal(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 190;
    int height = 128;
    tft.pushImage(x, y, width, height, scrapMetal);
    delay(2000);
}