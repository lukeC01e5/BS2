#include <Arduino.h>
#include "GlobalDefs.h"
#include "forestComplete.h"

// Declare or include the forest complete image data as a uint16_t array
// extern const uint16_t forestComplete[];

void drawForestComplete(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120;
    int height = 135;
    tft.pushImage(x, y, width, height, forestComplete);
    delay(200);
}