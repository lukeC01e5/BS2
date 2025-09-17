#include <Arduino.h>
#include "GlobalDefs.h"
#include "babyBird.h"

void drawBabyBird(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 128;
    tft.pushImage(x, y, width, height, babyBird);
    delay(2000);
}