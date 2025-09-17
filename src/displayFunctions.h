//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.h
#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Define TFT_GRAY if not provided by the library
#ifndef TFT_GRAY
#define TFT_GRAY 0x8410 // RGB565 value for gray
#endif

// Function declarations only (no definitions in header)
void drawAnimalImage(TFT_eSPI &tft, const char *creatureName);
void showMessageCenter(TFT_eSPI &tft, const String &message, uint16_t fgColor, uint16_t bgColor);
void showSmileyFace(TFT_eSPI &tft);
void lostLife(TFT_eSPI &tft, int oldLives, int newLives);
void lostLife(TFT_eSPI &tft, int wrongGuesses);
void handleZoneResource(TFT_eSPI &tft, const char *ZONE);

// Add missing resource function declarations
void drawWater(TFT_eSPI &tft);
void drawLava(TFT_eSPI &tft);
void drawWood(TFT_eSPI &tft);
void drawScrapMetal(TFT_eSPI &tft);
void drawKey(TFT_eSPI &tft);
void drawWaterComplete(TFT_eSPI &tft);
void drawLavaComplete(TFT_eSPI &tft);
void drawCityComplete(TFT_eSPI &tft);
void drawForestComplete(TFT_eSPI &tft);

// ONLY DECLARE the function here (don't define it)
void showRewardDisplay(TFT_eSPI &tft, const String &imageType, const String &displayText, int delayMs);

#endif