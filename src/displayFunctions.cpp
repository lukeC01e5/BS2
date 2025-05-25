//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.cpp
#include "displayFunctions.h"
#include <math.h> // for sin, cos

// Helper to draw a simple arc using many drawPixel calls
static void drawArc(TFT_eSPI &tft, int xCenter, int yCenter, int radius,
                    float startAngleDeg, float endAngleDeg, uint16_t color)
{
    // Convert degrees to radians
    float startRad = startAngleDeg * 3.14159f / 180.0f;
    float endRad = endAngleDeg * 3.14159f / 180.0f;

    // Step through angles, drawing pixels
    // Larger "step" = chunkier arc. Smaller "step" = smoother (slower).
    const float step = 0.02f;
    for (float theta = startRad; theta <= endRad; theta += step)
    {
        int x = xCenter + (int)(radius * cos(theta));
        int y = yCenter + (int)(radius * sin(theta));
        tft.drawPixel(x, y, color);
    }
}

void showMessageCenter(TFT_eSPI &tft, const String &message, uint16_t fgColor, uint16_t bgColor)
{
    tft.fillScreen(bgColor);
    tft.setTextColor(fgColor, bgColor);

    // Optionally adjust cursor to roughly center text
    int16_t x = 10;
    int16_t y = tft.height() / 2 - 10;

    tft.setCursor(x, y);
    tft.println(message);
}

void showSmileyFace(TFT_eSPI &tft)
{
    tft.fillScreen(TFT_BLACK);

    // Face center and radius
    int16_t centerX = tft.width() / 2;
    int16_t centerY = tft.height() / 2;
    int16_t faceRadius = min(tft.width(), tft.height()) / 4;

    // Draw the face in yellow
    tft.fillCircle(centerX, centerY, faceRadius, TFT_YELLOW);

    // Draw eyes
    int16_t eyeOffsetX = faceRadius / 2;
    int16_t eyeOffsetY = faceRadius / 3;
    int16_t eyeRadius = faceRadius / 8;
    tft.fillCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeRadius, TFT_BLACK);
    tft.fillCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeRadius, TFT_BLACK);

    // For an inverted arc (curve up), move the mouth center *above* centerY
    int16_t mouthCenterY = centerY - (faceRadius / 5);
    int16_t mouthRadius = faceRadius / 2;
    // Angles from about 20° to 160° draw a curve that opens upward
    drawArc(tft, centerX, mouthCenterY, mouthRadius, 20, 160, TFT_BLACK);

    // Show "Correct!!!" in green at the bottom
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(10, tft.height() - 30);
    tft.println(" Correct!!!");

    delay(2000);
}
