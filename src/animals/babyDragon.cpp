#include <Arduino.h>
#include "GlobalDefs.h"
#include "babyDragon.h"

void drawBabyDragon(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 120;
    int height = 128;
    tft.pushImage(x, y, width, height, babyDragon);
    delay(2000);
}