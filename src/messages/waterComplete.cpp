#include <Arduino.h>
#include "GlobalDefs.h"
#include "waterComplete.h"

// Declare or include the key image data as a uint16_t array
// extern const uint16_t keyImage[];

void drawWaterComplete(TFT_eSPI &tft)
{
    tft.setSwapBytes(true);

    int x = 0;
    int y = 0;
    int width = 120; // Match array size 16200 = 135×120
    int height = 135;

    tft.pushImage(x, y, width, height, waterComplete);

    tft.setSwapBytes(false);
}