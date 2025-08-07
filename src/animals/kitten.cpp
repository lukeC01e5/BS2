#include <Arduino.h>
#include "GlobalDefs.h"
#include "kitten.h"

void drawKitten(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 20;
    int y = 8;
    int width = 190;
    int height = 128;
    tft.pushImage(x, y, width, height, kitten);
    delay(2000);
}