//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.h
#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#include <Arduino.h> // or <Arduino.h>, <string>, etc. if needed #include <TFT_eSPI.h>

void drawAnimalImage(TFT_eSPI &tft, const char *creatureName); 

void showMessageCenter(TFT_eSPI &tft, const String &message, uint16_t fgColor, uint16_t bgColor);


void showSmileyFace(TFT_eSPI &tft);

void lostLife(TFT_eSPI &tft, int oldLives, int newLives);

void waterWon();



#endif