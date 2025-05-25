//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.h
#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * Show a message in the middle of the TFT screen.
 * param tft: reference to the TFT display
 * param message: text to display
 * param fgColor: foreground/text color
 * param bgColor: background fill color
 */
void showMessageCenter(TFT_eSPI &tft, const String &message, uint16_t fgColor, uint16_t bgColor);

/**
 * Clears the screen and shows a quick "Correct!" message in green.
 */
void showCorrectMsg(TFT_eSPI &tft);

/**
 * Clears the screen and shows a "Try again!" message in red.
 */
void showWrongMsg(TFT_eSPI &tft);

void showSmileyFace(TFT_eSPI &tft);

#endif