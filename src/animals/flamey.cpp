#include <Arduino.h>
#include "GlobalDefs.h"
#include "flamey.h"

void drawFlamey(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, flamey);
    delay(2000);
}