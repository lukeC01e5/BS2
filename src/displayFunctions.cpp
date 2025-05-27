//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.cpp

#include "displayFunctions.h"
#include <math.h> // for sin, cos
#include "animals/squidy.h"
#include "animals/orca.h"
#include "animals/water.h"
#include "GlobalDefs.h"
#include <Arduino.h> // or <Arduino.h>, <string>, etc. if needed #include <TFT_eSPI.h>

void waterWon()
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text for "Prize won"
    tft.println("\n Prize won");
    tft.setTextColor(TFT_BLUE, TFT_BLACK); // Blue text for "WATER!"
    tft.println(" WATER!");
    drawWater(tft);
}

void drawAnimalImage(TFT_eSPI &tft, const char *creatureName)
{
    if (strcmp(creatureName, "SQUIDY") == 0)
    {
        tft.fillScreen(TFT_BLUE);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
        tft.println("SQUIDY IMAGE PLACEHOLDER");
        drawSquidy(tft);
    }
    else if (strcmp(creatureName, "ORCA") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.println("ORCA IMAGE PLACEHOLDER");
        drawOrca(tft);
    }

    else
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("No image available");
    }
}

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

/** Draw a small heart at (x, y). Adjust shape/size as you like. */
static void drawHeart(TFT_eSPI &tft, int x, int y, uint16_t color)
{
    // Simple approach: two small circles + triangle
    // Adjust radius/coordinates to get a nicer shape
    int r = 6;
    // Left circle
    tft.fillCircle(x - r, y, r, color);
    // Right circle
    tft.fillCircle(x + r, y, r, color);
    // Triangle/rectangle
    tft.fillTriangle(x - (2 * r), y, x + (2 * r), y, x, y + (2 * r), color);
}

void lostLife(TFT_eSPI &tft, int oldLives, int newLives)
{
    // If we actually lost a life (oldLives > newLives), blink that last heart
    if (oldLives > newLives && oldLives <= 9)
    {
        int lostHeartIndex = oldLives; // 1-based indexing
        for (int i = 0; i < 2; i++)
        {
            // Compute grid cell for the heart just lost
            int idx = lostHeartIndex - 1;
            int row = idx / 3;
            int col = idx % 3;
            int cellW = tft.width() / 3;
            int cellH = tft.height() / 3;
            int16_t hx = col * cellW + cellW / 2;
            int16_t hy = row * cellH + cellH / 2;

            // Blink red → black
            uint16_t color = (i % 2 == 0) ? TFT_RED : TFT_BLACK;
            drawHeart(tft, hx, hy, color);
            delay(2000); // Shorter blink
        }
    }

    // Draw hearts for newLives (up to 9)
    int heartsToDraw = (newLives > 9) ? 9 : newLives;
    for (int i = 1; i <= heartsToDraw; i++)
    {
        int idx = i - 1;
        int row = idx / 3;
        int col = idx % 3;
        int cellW = tft.width() / 3;
        int cellH = tft.height() / 3;
        int16_t hx = col * cellW + cellW / 2;
        int16_t hy = row * cellH + cellH / 2;
        drawHeart(tft, hx, hy, TFT_RED);
    }
}

void lostLife(TFT_eSPI &tft, int wrongGuesses)
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    if (wrongGuesses <= 0)
    {
        // Display "No lives left" in red
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(tft.width() / 4, tft.height() / 2); // Center the text roughly
        tft.println(" No lives\n left");
        delay(2000); // Show for 2 seconds
        return;      // Exit the function since no hearts need to be drawn
    }

    if (wrongGuesses > 9)
        wrongGuesses = 9;

    // Draw 'wrongGuesses' hearts in a 3×3 grid
    for (int i = 0; i < wrongGuesses; i++)
    {
        int row = i / 3;
        int col = i % 3;
        int cellW = tft.width() / 3;
        int cellH = tft.height() / 3;
        int16_t hx = col * cellW + cellW / 2;
        int16_t hy = row * cellH + cellH / 2;
        drawHeart(tft, hx, hy, TFT_RED);
    }
    delay(2000); // Show for 2 seconds
}
