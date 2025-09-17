#include <Arduino.h>
// #include <TFT_eSPI.h>
#include "squidy.h"
// #include "orca.h" // Assuming you have an orca.h file similar to squidy.h

#include "GlobalDefs.h"

void drawSquidy(TFT_eSPI &tft)
{
    // Fill the background:
    tft.fillScreen(TFT_BLACK);

    // Some placeholder text:
    tft.setCursor(0, 0);
    // tft.setTextColor(TFT_WHITE, TFT_BLUE);
    // tft.println("SQUIDY IMAGE PLACEHOLDER");

    tft.setSwapBytes(true);

    // Example: push an image from array data
    int x = 0;
    int y = 0;
    int width = 120;
    int height = 128;
    tft.pushImage(x, y, width, height, squidy); // If 'squidy' array is defined
    delay(2000);                                // Show for 2 seconds
}
