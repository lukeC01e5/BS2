//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\displayFunctions.cpp
#include <Arduino.h> // or <Arduino.h>, <string>, etc. if needed #include <TFT_eSPI.h>
#include "displayFunctions.h"
#include <math.h> // for sin, cos
#include "GlobalDefs.h"

#include "animals/squidy.h"
#include "animals/orca.h"
#include "animals/babyShark.h"
#include "animals/babyRay.h"

#include "animals/kitten.h"
#include "animals/puppy.h"
#include "animals/babyTrex.h"

#include "animals/flamingo.h"
#include "animals/flameLily.h"
#include "animals/babyDragon.h"

#include "animals/babyUnicorn.h"
#include "animals/bearCub.h"
#include "animals/babyBird.h"

#include "animals/botBot.h"
#include "animals/flamey.h"
#include "animals/bearCub.h"
#include "animals/sprouty.h"
#include "animals/tadpole.h"

#include "resources/wood.h"
#include "resources/scrapMetal.h"
#include "resources/water.h" // Unchanged
#include "resources/lava.h"  // Unchanged

#include "messages/key.h"
#include "messages/waterComplete.h"
#include "messages/lavaComplete.h"
#include "messages/cityComplete.h"
#include "messages/forestComplete.h" // Note: you have both forestComplete.h and forrestComplete.cpp

// Define a gray color constant if not provided by TFT_eSPI
#ifndef TFT_GRAY
#define TFT_GRAY 0x8410 // RGB565 value for gray
#endif

void drawAnimalImage(TFT_eSPI &tft, const char *creatureName)
{
    Serial.println("[DEBUG] drawAnimalImage called with: " + String(creatureName));

    if (strcmp(creatureName, "SQUIDY") == 0)
    {
        tft.fillScreen(TFT_BLUE);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
        // tft.println("SQUIDY IMAGE PLACEHOLDER");
        drawSquidy(tft); // Should be: drawSquidy(tft);
    }
    else if (strcmp(creatureName, "ORCA") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println("ORCA IMAGE PLACEHOLDER");
        drawOrca(tft); // Should be: drawOrca(tft);
    }
    else if (strcmp(creatureName, "BABYSHARK") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println("BABYSHARK IMAGE PLACEHOLDER");
        drawBabyShark(tft); // Should be: drawBabyShark(tft);
    }
    else if (strcmp(creatureName, "BABY_RAY") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" BABY_RAY IMAGE PLACEHOLDER");
        drawBabyRay(tft); // Should be: drawBabyRay(tft);
    }
    else if (strcmp(creatureName, "KITTEN") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" KITTEN IMAGE PLACEHOLDER");
        drawKitten(tft); // Should be: drawKitten(tft);
    }
    else if (strcmp(creatureName, "PUP") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" PUP IMAGE PLACEHOLDER");
        drawPuppy(tft); // Should be: drawPuppy(tft);
    }
    else if (strcmp(creatureName, "DINO_EGG") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" DINO_EGG IMAGE PLACEHOLDER");
        drawBabyTrex(tft); // Should be: drawBabyTrex(tft);
    }
    else if (strcmp(creatureName, "FLAMINGO") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" FLAMINGO IMAGE PLACEHOLDER");
        drawFlamingo(tft); // Should be: drawFlamingo(tft);
    }
    else if (strcmp(creatureName, "FLAME_LILY") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" FLAME_LILY IMAGE PLACEHOLDER");
        drawFlameLily(tft); // Should be: drawFlameLily(tft);
    }
    else if (strcmp(creatureName, "BABY_DRAGON") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" BABY_DRAGON IMAGE PLACEHOLDER");
        drawBabyDragon(tft); // Should be: drawBabyDragon(tft);
    }
    else if (strcmp(creatureName, "UNICORN") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" UNICORN IMAGE PLACEHOLDER");
        drawBabyUnicorn(tft); // Should be: drawBabyUnicorn(tft);
    }
    else if (strcmp(creatureName, "BEAR_CUB") == 0)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" BEAR_CUB IMAGE PLACEHOLDER");
        drawBearCub(tft); // Should be: drawBearCub(tft);
    }
    else if (strcmp(creatureName, "BIRDY") == 0)
    {
        Serial.println("[DEBUG] BIRDY matched! Calling drawBabyBird...");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" BIRDY IMAGE PLACEHOLDER");
        // delay(1000); // Give time to see the placeholder text
        drawBabyBird(tft);
        Serial.println("[DEBUG] drawBabyBird call completed");
    }
    else if (strcmp(creatureName, "BOTBOT") == 0)
    {
        Serial.println("[DEBUG] BOTBOT matched! Calling drawBotBot...");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" BOTBOT IMAGE PLACEHOLDER");
        // delay(1000); // Give time to see the placeholder text
        drawBotBot(tft);
    }
    else if (strcmp(creatureName, "FLAMEY") == 0)
    {
        Serial.println("[DEBUG] FLAMEY matched! Calling drawFlamey...");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" FLAMEY IMAGE PLACEHOLDER");
        // delay(1000); // Give time to see the placeholder text
        drawFlamey(tft);
    }
    else if (strcmp(creatureName, "SPROUTY") == 0)
    {
        Serial.println("[DEBUG] SPROUTY matched! Calling drawSprouty...");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" SPROUTY IMAGE PLACEHOLDER");
        // delay(1000); // Give time to see the placeholder text
        drawSprouty(tft);
    }
    else if (strcmp(creatureName, "TADPOLE") == 0)
    {
        Serial.println("[DEBUG] TADPOLE matched! Calling drawTadpole...");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tft.println(" TADPOLE IMAGE PLACEHOLDER");
        // delay(1000); // Give time to see the placeholder text
        drawTadpole(tft);
    }
    else
    {
        Serial.println("[DEBUG] No match found for creature: " + String(creatureName));
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("No image for: ");
        tft.println(creatureName);
        delay(2000);
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
    tft.setRotation(1); // ADD THIS - 240x135
    tft.fillScreen(TFT_BLACK);

    // Smaller smiley for small screen
    int16_t centerX = 120;   // CHANGE - Center of 240px width
    int16_t centerY = 45;    // CHANGE - Upper third of 135px height
    int16_t faceRadius = 30; // CHANGE - Smaller radius

    // Draw the face in yellow
    tft.fillCircle(centerX, centerY, faceRadius, TFT_YELLOW);

    // Draw eyes - ADJUST proportions
    int16_t eyeOffsetX = faceRadius / 2;
    int16_t eyeOffsetY = faceRadius / 3;
    int16_t eyeRadius = faceRadius / 8;
    tft.fillCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeRadius, TFT_BLACK);
    tft.fillCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeRadius, TFT_BLACK);

    // Draw mouth - ADJUST position
    int16_t mouthCenterY = centerY - (faceRadius / 5);
    int16_t mouthRadius = faceRadius / 2;
    drawArc(tft, centerX, mouthCenterY, mouthRadius, 20, 160, TFT_BLACK);

    // Show "Correct!!!" in green at the bottom - ADJUST position and size
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);     // CHANGE - Smaller text
    tft.setCursor(60, 100); // CHANGE - Better position for 240x135
    tft.println("Correct!");

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

void handleZoneResource(TFT_eSPI &tft, const char *ZONE)
{
    String resourceCode;
    String resourceName;

    if (strcmp(ZONE, "lava") == 0)
    {
        resourceCode = "LA";
        resourceName = "Lava";
    }
    else if (strcmp(ZONE, "forest") == 0)
    {
        resourceCode = "PL";     // Changed from "FO"
        resourceName = "Plants"; // Changed from "Forest"
    }
    else if (strcmp(ZONE, "city") == 0)
    {
        resourceCode = "MT";    // Changed from "CI"
        resourceName = "Metal"; // Changed from "City"
    }
    else if (strcmp(ZONE, "Water") == 0)
    {
        resourceCode = "WA";
        resourceName = "Water";
    }
}

void showRewardDisplay(TFT_eSPI &tft, const String &imageType, const String &displayText, int delayMs)
{
    tft.setRotation(1); // 240x135 landscape
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true); // Fix for image display

    // Draw image on LEFT half (0-119 pixels wide, full height 135)
    if (imageType == "WA" || imageType == "water")
    {
        // Use ORIGINAL water resource image (from src/resources/)
        tft.fillRect(0, 0, 120, 135, TFT_BLUE);
        drawWater(tft); // ← FIXED! Use original resource image
    }
    else if (imageType == "LA" || imageType == "lava")
    {
        // Use ORIGINAL lava resource image (from src/resources/)
        tft.fillRect(0, 0, 120, 135, TFT_RED);
        drawLava(tft); // ← FIXED! Use original resource image
    }
    else if (imageType == "PL" || imageType == "Plants" || imageType == "wood")
    {
        // Use ORIGINAL wood/plants resource image (from src/resources/)
        tft.fillRect(0, 0, 120, 135, TFT_GREEN);
        drawWood(tft); // ← FIXED! Use original resource image
    }
    else if (imageType == "MT" || imageType == "Metal" || imageType == "city")
    {
        // Use ORIGINAL scrap metal resource image (from src/resources/)
        tft.fillRect(0, 0, 120, 135, TFT_GRAY);
        drawScrapMetal(tft); // ← FIXED! Use original resource image
    }
    else if (imageType == "KEY")
    {
        tft.fillRect(0, 0, 120, 135, TFT_BLACK);
        drawKey(tft);
    }
    else if (imageType == "ZONE_COMPLETE")
    {
        Serial.println("[DEBUG] ZONE_COMPLETE triggered");
        Serial.println("[DEBUG] Display text: " + displayText);

        // Use COMPLETION images (from src/messages/) ONLY for zone already completed
        if (displayText.indexOf("water") != -1) // ← CHANGED to lowercase
        {
            Serial.println("[DEBUG] Calling drawWaterComplete()");
            tft.fillRect(0, 0, 120, 135, TFT_BLUE);
            drawWaterComplete(tft);
        }
        else if (displayText.indexOf("lava") != -1) // ← CHANGED to lowercase
        {
            Serial.println("[DEBUG] Calling drawLavaComplete()");
            tft.fillRect(0, 0, 120, 135, TFT_RED);
            drawLavaComplete(tft);
        }
        else if (displayText.indexOf("forest") != -1) // ← CHANGED to lowercase
        {
            Serial.println("[DEBUG] Calling drawForestComplete()");
            tft.fillRect(0, 0, 120, 135, TFT_GREEN);
            drawForestComplete(tft);
        }
        else if (displayText.indexOf("city") != -1) // ← CHANGED to lowercase
        {
            Serial.println("[DEBUG] Calling drawCityComplete()");
            tft.fillRect(0, 0, 120, 135, TFT_GRAY);
            drawCityComplete(tft);
        }
        else
        {
            Serial.println("[DEBUG] No zone match found, using fallback");
            // Fallback: generic completion symbol
            tft.fillRect(0, 0, 120, 135, TFT_BLUE);
            tft.setTextColor(TFT_WHITE, TFT_BLUE);
            tft.setTextSize(3);
            tft.setCursor(30, 50);
            tft.println("✓");
        }
    }
    else if (imageType.startsWith("CREATURE:"))
    {
        String creatureName = imageType.substring(9);
        tft.fillRect(0, 0, 120, 135, TFT_BLACK);
        drawAnimalImage(tft, creatureName.c_str());
    }
    else
    {
        // Default: just fill left half with black
        tft.fillRect(0, 0, 120, 135, TFT_BLACK);
    }

    tft.setSwapBytes(false); // Reset after images

    // Draw vertical divider line
    tft.drawLine(120, 0, 120, 135, TFT_WHITE);

    // Draw text on RIGHT half (120-239 pixels wide)
    // Center the text vertically and horizontally in the right half

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Calculate text positioning for centering
    int16_t textWidth = displayText.length() * 12; // Approximate width (6 pixels per char * 2 for size 2)
    int16_t textHeight = 16;                       // Height for text size 2

    // Count number of lines (split by \n)
    int lineCount = 1;
    for (int i = 0; i < displayText.length(); i++)
    {
        if (displayText.charAt(i) == '\n')
            lineCount++;
    }

    int totalTextHeight = lineCount * textHeight;

    // Center vertically in right half
    int16_t startY = (135 - totalTextHeight) / 2;

    // Split text by newlines and center each line
    String currentLine = "";
    int lineNum = 0;

    for (int i = 0; i <= displayText.length(); i++)
    {
        char c = (i < displayText.length()) ? displayText.charAt(i) : '\n'; // Add final newline

        if (c == '\n' || i == displayText.length())
        {
            // Center this line horizontally in right half
            int16_t lineWidth = currentLine.length() * 12; // Approximate width
            int16_t startX = 120 + (120 - lineWidth) / 2;  // Center in right half (120px wide)

            tft.setCursor(startX, startY + (lineNum * textHeight));
            tft.print(currentLine);

            currentLine = "";
            lineNum++;
        }
        else
        {
            currentLine += c;
        }
    }

    delay(delayMs); // Use the provided delay instead of fixed 1500ms
}
