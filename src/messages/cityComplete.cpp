#include <Arduino.h>
#include "GlobalDefs.h"
#include "cityComplete.h"

// Declare or include the city complete image data as a uint16_t array
// extern const uint16_t cityCompleteImage[];

void drawCityComplete(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 128;
    tft.pushImage(x, y, width, height, cityComplete);
    delay(200);
}