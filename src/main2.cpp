#include <Keypad.h>
#include <Arduino.h>
#include <SPI.h>
#include "GlobalDefs.h"
#include "quizAnswers.h"
#include "quizQuestions.h"
#include "animals.h"
#include <esp_system.h> // For esp_restart
#include "displayFunctions.h"
#include <Preferences.h>

Preferences preferences;

// #include "animals/squidy.h"

const char *ZONE_LIST[] = {"water", "lava", "forest", "city"};
const int ZONE_COUNT = 4;
int currentZoneIndex = 0;

// #define ZONE "water"

// #pragma once

// Keypad configuration (4x1 keypad)
const byte ROWS = 4;
const byte COLS = 1;
char keys[ROWS][COLS] = {
    {'1'},  // red
    {'2'},  // green
    {'3'},  // blue
    {'4'}}; // yellow;

byte rowPins[ROWS] = {21, 17, 2, 15};
byte colPins[COLS] = {12};
Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// TFT and RFID objects

struct UserTag
{
    String contents;
};

struct Player
{
    int challengeCode;
    int wrongGuesses;
    int boolVal;
    String playerName;
};

int quizDisplay(int codeValue, int questionNumber);
String formatCode(int codeValue);
int randomInRangeExclude(int minRange, int maxRange, int excludeVal);
bool directWriteRFID(MFRC522 &rfid, MFRC522::MIFARE_Key &key, const String &data, byte blockNumber);
void secretZoneMenu();
void lostLife(TFT_eSPI &tft, int wrongGuesses);

// A simple function to simulate adding 5 coin to a remote DB
bool postToDBAdd5Coin()
{
    Serial.println("[DEBUG] Simulating postToDBAdd5Coin...");
    return true; // For now, just pretend it always succeeds
}

// Return a random creature from ANIMALS[] that matches ID & zone
AnimalInfo pickRandomCreature(int desiredId, const char *zone)
{
    const int animalCount = sizeof(ANIMALS) / sizeof(ANIMALS[0]);
    AnimalInfo candidates[animalCount];
    int count = 0;
    for (int i = 0; i < animalCount; i++)
    {
        if (ANIMALS[i].id == desiredId && strcmp(ANIMALS[i].environment, zone) == 0)
        {
            candidates[count++] = ANIMALS[i];
        }
    }
    if (count == 0)
        return {"NO_CREATURE", 1, "normal", "NC"};
    int index = random(0, count);
    return candidates[index];
}

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Starting Setup ===");

    tft.init();
    tft.setRotation(1);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);

    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    //////////////////////////////////////////////////

    preferences.begin("game", false);
    String savedZone = preferences.getString("zone", "water");
    for (int i = 0; i < ZONE_COUNT; i++)
    {
        if (savedZone == ZONE_LIST[i])
        {
            currentZoneIndex = i;
            break;
        }
    }
#define ZONE ZONE_LIST[currentZoneIndex]
    Serial.println("[DEBUG] Loaded ZONE: " + String(ZONE));

    //////////////////////////////////////////////

    // Set factory key bytes to 0xFF
    for (int i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    // Initialize MFRC522
    mfrc522.PCD_Init();
    delay(1000);

    Serial.println("=== Setup Complete ===\n");
}

void loop()
{
    ////////////////////////////////////////////////
    // Check for secret combo: buttons 1 and 4 pressed together
    // Check for long press on button 1 (3 seconds)

    // Check for secret menu: button 1 pressed

    ////////////////////////////////////

    // Create a UserTag object
    UserTag userTag;

    // Prompt for RFID
    tft.fillScreen(TFT_BLACK);
    tft.println("\n Insert Key");
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        char k = keypad.getKey();
        if (k == '1')
        {
            secretZoneMenu();
            return;
        }
        delay(100);
    }
    tft.println("Tag present!");

    // Authenticate
    byte trailerBlock = 3;
    MFRC522::StatusCode authStatus = mfrc522.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (authStatus != MFRC522::STATUS_OK)
    {
        Serial.println("Auth failed: " + String(mfrc522.GetStatusCodeName(authStatus)));
        tft.println("Auth Failed");
        delay(2000);
        return;
    }

    // Read block 1 (player data)
    byte buffer[18];
    byte size = sizeof(buffer);
    MFRC522::StatusCode readStatus = mfrc522.MIFARE_Read(1, buffer, &size);
    if (readStatus != MFRC522::STATUS_OK)
    {
        Serial.println("Read Block1 failed: " + String(mfrc522.GetStatusCodeName(readStatus)));
        tft.println("Read Block1 Failed");
        delay(2000);
        return;
    }

    // Convert block1 data to string
    String block1Data;
    for (int i = 0; i < 16; i++)
    {
        if (buffer[i] == 0)
            break;
        block1Data += (char)buffer[i];
    }
    userTag.contents = block1Data;

    // Parse into Player struct
    Player player;
    if (block1Data.length() < 7)
    {
        Serial.println("Data too short!");
        tft.println("Data too short!");
    }
    else
    {
        player.challengeCode = block1Data.substring(0, 3).toInt();
        char wg = block1Data.charAt(3);
        player.wrongGuesses = (wg >= '0' && wg <= '9') ? wg - '0' : 0;

        int qIndex = block1Data.indexOf('?');
        int pIndex = block1Data.indexOf('%');
        if (qIndex != -1 && pIndex != -1 && qIndex < pIndex)
        {
            // Now boolVal is a single digit
            player.boolVal = block1Data.substring(qIndex + 1, qIndex + 2).toInt();
            // Loot codes are after %
            player.playerName = block1Data.substring(pIndex + 1, pIndex + 9); // 8 chars
        }
    }

    if (player.wrongGuesses <= 0)
    {
        // Display "No lives left" in red
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(tft.width() / 4, tft.height() / 2); // Center the text roughly
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(0, 0);
        tft.println("\n No lives\n left");

        delay(3000);   // Wait for 3 seconds before restarting
        esp_restart(); // Restart the program
    }

    // ...after reading userTag.contents...

    // 1. Find the code area after '%'
    int percentIdx = userTag.contents.indexOf('%');
    String codeArea = "";
    if (percentIdx != -1 && userTag.contents.length() > percentIdx + 1)
    {
        int codeStart = percentIdx + 1;
        int codeEnd = codeStart;
        // Only count alpha codes (creature/resource codes)
        while (codeEnd < userTag.contents.length() && isAlpha(userTag.contents.charAt(codeEnd)))
        {
            codeEnd++;
        }
        codeArea = userTag.contents.substring(codeStart, codeEnd);
    }

    // 2. Check if there is space (let's say max 7 codes, 2 chars each = 14 chars)
    const int MAX_CODES = 4; // 4 codes, 2 chars each = 8 chars
    if (codeArea.length() >= MAX_CODES * 2)
    {
        tft.fillScreen(TFT_RED);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.println("All slots full!");
        tft.println("Tag:");
        tft.println(userTag.contents);
        Serial.println("[ERROR] All slots full! Tag: " + userTag.contents);
        delay(4000);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }

    // Gather answers for each question
    String val1, val2, val3;
    if (player.challengeCode >= 0 && player.challengeCode < 50)
    {
        val1 = quizAnswers[player.challengeCode].value1;
        val2 = quizAnswers[player.challengeCode].value2;
        val3 = quizAnswers[player.challengeCode].value3;
    }
    else
    {
        val1 = "(Out of Range)";
        val2 = "(Out of Range)";
        val3 = "(Out of Range)";
    }
    String questionVals[3] = {val1, val2, val3};

    // Ask 3 questions
    for (int qIndex = 0; qIndex < 3; qIndex++)
    {
        bool doneWithThisQuestion = false;
        while (!doneWithThisQuestion)
        {
            int thisCode = questionVals[qIndex].toInt();
            Serial.println("[DEBUG] Asking question # " + String(qIndex + 1) +
                           " with codeValue=" + String(thisCode));

            // Return which box is correct
            int correctIndex = quizDisplay(thisCode, qIndex + 1);

            while (true)
            {
                char k = keypad.getKey();
                if (k == NO_KEY)
                {
                    delay(50);
                    continue;
                }
                Serial.println("[DEBUG] User pressed key: " + String(k));

                // map '1','2','3','4' to box indices 0..3
                int pressedBox = -1;
                if (k == '1')
                    pressedBox = 0;
                else if (k == '2')
                    pressedBox = 1;
                else if (k == '3')
                    pressedBox = 2;
                else if (k == '4')
                    pressedBox = 3;

                if (pressedBox == correctIndex)
                {
                    showSmileyFace(tft);
                    // Correct!
                    tft.fillScreen(TFT_BLACK);
                    tft.setCursor(0, 0);
                    tft.setTextColor(TFT_GREEN, TFT_BLACK);
                    tft.println("Correct!");
                    Serial.println("[DEBUG] User answered question # " +
                                   String(qIndex + 1) + " correctly.");
                    doneWithThisQuestion = true;
                }
                else
                {
                    player.wrongGuesses--;
                    // Wrong => lose 1 life
                    lostLife(tft, player.wrongGuesses);
                    tft.fillScreen(TFT_BLACK);
                    tft.setCursor(0, 0);
                    tft.setTextColor(TFT_RED, TFT_BLACK);
                    tft.println("\n Try again!");
                    Serial.println("[DEBUG] Wrong answer. wrongGuesses=" + String(player.wrongGuesses));
                    delay(1000);
                }
                break;
            }
        }
    }

    // If all 3 answered, add 5 coin to DB
    bool postWorked = postToDBAdd5Coin();
    if (postWorked)
    {
        // Mark player.boolVal=15 as “task done”
        player.boolVal = 15;

        // Possibly add a random creature
        int randomVal = random(0, 31);
        String creatureCode = "";
        String creatureName = "";
        if (randomVal <= 18)
        {
            String resourceCode = "";
            String resourceName = "";

            if (strcmp(ZONE, "water") == 0)
            {
                resourceCode = "WA";
                resourceName = "Water";
            }
            else if (strcmp(ZONE, "lava") == 0)
            {
                resourceCode = "LA";
                resourceName = "Lava";
            }
            else if (strcmp(ZONE, "forest") == 0)
            {
                resourceCode = "FO";
                resourceName = "Forest";
            }
            else if (strcmp(ZONE, "city") == 0)
            {
                resourceCode = "CI";
                resourceName = "City";
            }

            if (resourceCode != "")
            {
                // Show resource win on TFT
                tft.fillScreen(TFT_BLUE);
                tft.setCursor(0, 0);
                tft.setTextColor(TFT_WHITE, TFT_BLUE);
                tft.println(" Resource\n Won:\n " + resourceName);
                delay(2000);

                // 1. Extract existing codes after '%'
                String existingCodes = "";
                int percentIdx = userTag.contents.indexOf('%');
                if (percentIdx != -1 && userTag.contents.length() > percentIdx + 1)
                {
                    int codeStart = percentIdx + 1;
                    int codeEnd = codeStart;
                    while (codeEnd < userTag.contents.length() && isAlpha(userTag.contents.charAt(codeEnd)))
                    {
                        codeEnd++;
                    }
                    existingCodes = userTag.contents.substring(codeStart, codeEnd);
                }

                // 2. Append new resource code if not already present
                if (existingCodes.indexOf(resourceCode) == -1)
                {
                    existingCodes += resourceCode;
                }

                // 3. Build the new RFID data string
                char wgChar = (player.wrongGuesses >= 0 && player.wrongGuesses <= 9) ? ('0' + player.wrongGuesses) : '0';
                String newBlockData = formatCode(player.challengeCode) + wgChar +
                                      "?" + String(player.boolVal) + "%" + existingCodes;

                // Pad with zeros if necessary
                while (newBlockData.length() < 16)
                    newBlockData += "0";

                Serial.println("[DEBUG] Original Tag: " + userTag.contents);
                Serial.println("[DEBUG] New Tag    : " + newBlockData);

                // Write to RFID block 1
                if (directWriteRFID(mfrc522, key, newBlockData, 1))
                    Serial.println("[DEBUG] Block write success with new resource code!");
                else
                    Serial.println("[DEBUG] Block write failed!");
            }
        }
        else
        {
            // ID=2 creature
            AnimalInfo chosen = pickRandomCreature(2, ZONE);
            creatureCode = chosen.code;
            creatureName = chosen.name;

            tft.fillScreen(TFT_GREEN);
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.println(" Creature\n Won:\n " + creatureName);
            delay(2000); // give the user 2 seconds to see the name

            drawAnimalImage(tft, creatureName.c_str());

            // --- Add RFID update for creature code ---
            // 1. Extract existing codes after '%'
            String existingCodes = "";
            int percentIdx = userTag.contents.indexOf('%');
            if (percentIdx != -1 && userTag.contents.length() > percentIdx + 1)
            {
                int codeStart = percentIdx + 1;
                int codeEnd = codeStart;
                while (codeEnd < userTag.contents.length() && isAlpha(userTag.contents.charAt(codeEnd)))
                {
                    codeEnd++;
                }
                existingCodes = userTag.contents.substring(codeStart, codeEnd);
            }

            Serial.println("[DEBUG] Existing codes before creature: " + existingCodes);

            // 2. Append new creature code if not already present
            if (existingCodes.indexOf(creatureCode) == -1)
            {
                existingCodes += creatureCode;
                Serial.println("[DEBUG] Appending creature code: " + creatureCode);
            }
            else
            {
                Serial.println("[DEBUG] Creature code already present: " + creatureCode);
            }

            // 3. Build the new RFID data string
            char wgChar = (player.wrongGuesses >= 0 && player.wrongGuesses <= 9) ? ('0' + player.wrongGuesses) : '0';
            String newBlockData = formatCode(player.challengeCode) + wgChar +
                                  "?" + String(player.boolVal) + "%" + existingCodes;

            // Pad with zeros if necessary
            while (newBlockData.length() < 16)
                newBlockData += "0";

            Serial.println("[DEBUG] Original Tag: " + userTag.contents);
            Serial.println("[DEBUG] New Tag    : " + newBlockData);

            // Write to RFID block 1
            if (directWriteRFID(mfrc522, key, newBlockData, 1))
                Serial.println("[DEBUG] Block write success with new creature code!");
            else
                Serial.println("[DEBUG] Block write failed!");
        }
    }
    else
    {
        Serial.println("[DEBUG] postWorked == false; skipping RFID write...");
    }

    // Halt card
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    Serial.println("[DEBUG] Halted and stopped crypto.");

    // Reboot after a short delay
    delay(3000);
    tft.fillScreen(TFT_BLACK);
    Serial.println("=== End Loop, rebooting now ===");

    esp_restart();
}

/**
 * Produce a 3-digit string (e.g. "005") from codeValue
 */
String formatCode(int codeValue)
{
    if (codeValue < 0)
        codeValue = 0;
    char buf[5];
    snprintf(buf, sizeof(buf), "%03d", codeValue);
    return String(buf);
}

int randomInRangeExclude(int minRange, int maxRange, int excludeVal)
{
    if (maxRange < minRange)
        return excludeVal;
    int val;
    do
    {
        val = random(minRange, maxRange + 1);
    } while (val == excludeVal);
    return val;
}

// Build a question prompt
String buildQuestionPrompt(int codeValue, int questionNumber)
{
    if (codeValue >= 0 && codeValue <= 20)
    {
        int qIndex = questionNumber - 1;
        if (qIndex < 0 || qIndex > 2)
            qIndex = 0;
        switch (qIndex)
        {
        case 0:
            return questionSheet[codeValue].q1;
        case 1:
            return questionSheet[codeValue].q2;
        case 2:
            return questionSheet[codeValue].q3;
        }
    }
    return "Question " + String(questionNumber) + ":";
}

/**
 * Draw question screen, return the correct box index.
 */
int quizDisplay(int codeValue, int questionNumber)
{
    Serial.println("[DEBUG] quizDisplay() code=" + String(codeValue) +
                   ", Q num=" + String(questionNumber));

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    String questionText = buildQuestionPrompt(codeValue, questionNumber);
    tft.println(questionText);

    int lineY = tft.height() / 5;
    tft.drawLine(0, lineY, tft.width(), lineY, TFT_WHITE);

    String correctAnswer;
    if (codeValue >= 0 && codeValue < 50)
    {
        if (questionNumber == 1)
            correctAnswer = quizAnswers[codeValue].value1;
        else if (questionNumber == 2)
            correctAnswer = quizAnswers[codeValue].value2;
        else
            correctAnswer = quizAnswers[codeValue].value3;
    }
    else
    {
        correctAnswer = "0";
    }

    // Build 4 answer options (1 correct, 3 distractors)
    String answerOptions[4];
    answerOptions[0] = correctAnswer;
    for (int i = 1; i < 4; i++)
    {
        String distractor;
        while (true)
        {
            int randIndex = random(0, 50);
            int randValIndex = random(0, 3);
            if (randValIndex == 0)
                distractor = quizAnswers[randIndex].value1;
            else if (randValIndex == 1)
                distractor = quizAnswers[randIndex].value2;
            else
                distractor = quizAnswers[randIndex].value3;

            if (distractor != correctAnswer)
                break;
        }
        answerOptions[i] = distractor;
    }
    // Shuffle them
    for (int i = 0; i < 4; i++)
    {
        int r = random(i, 4);
        String temp = answerOptions[i];
        answerOptions[i] = answerOptions[r];
        answerOptions[r] = temp;
    }

    int correctBoxIndex = 0;
    for (int i = 0; i < 4; i++)
    {
        if (answerOptions[i] == correctAnswer)
        {
            correctBoxIndex = i;
            break;
        }
    }

    const uint16_t boxColors[4] = {TFT_RED, TFT_YELLOW, TFT_BLUE, TFT_GREEN};
    int regionH = tft.height() - lineY;
    int regionW = tft.width();
    int boxW = regionW / 2;
    int boxH = regionH / 2;

    for (int i = 0; i < 4; i++)
    {
        int x = (i % 2) * boxW;
        int y = lineY + (i / 2) * boxH;
        tft.fillRect(x, y, boxW, boxH, boxColors[i]);
        tft.setTextColor(TFT_BLACK, boxColors[i]);
        tft.setCursor(x + 10, y + boxH / 2);
        tft.print(answerOptions[i]);
    }

    Serial.println("[DEBUG] correctAnswer=" + correctAnswer +
                   ", correctBoxIndex=" + String(correctBoxIndex));
    return correctBoxIndex;
}

/**
 * Writes data directly to an RFID block
 */
bool directWriteRFID(MFRC522 &rfid, MFRC522::MIFARE_Key &key, const String &data, byte blockNumber)
{
    byte trailerBlock = blockNumber + 3 - (blockNumber % 4);
    MFRC522::StatusCode status = rfid.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(rfid.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.println("Authentication failed: " + String(rfid.GetStatusCodeName(status)));
        return false;
    }
    byte blockContent[16];
    memset(blockContent, 0, sizeof(blockContent));
    data.getBytes(blockContent, 16);

    status = rfid.MIFARE_Write(blockNumber, blockContent, 16);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.println("Write failed: " + String(rfid.GetStatusCodeName(status)));
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        return false;
    }

    Serial.println("Write success for block " + String(blockNumber));
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return true;
}

void secretZoneMenu()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.println("Secret Zone Menu");
    tft.println("Use 2/3 to scroll");
    tft.println("Press 4 to confirm");

    int menuZoneIndex = currentZoneIndex;
    bool confirmed = false;
    while (!confirmed)
    {
        tft.fillRect(0, 80, tft.width(), 40, TFT_BLACK);
        tft.setCursor(0, 80);
        tft.println("ZONE: " + String(ZONE_LIST[menuZoneIndex]));

        char k = keypad.getKey();
        if (k == '2')
        { // Scroll left
            menuZoneIndex = (menuZoneIndex - 1 + ZONE_COUNT) % ZONE_COUNT;
            delay(200);
        }
        else if (k == '3')
        { // Scroll right
            menuZoneIndex = (menuZoneIndex + 1) % ZONE_COUNT;
            delay(200);
        }
        else if (k == '4')
        { // Confirm
            preferences.putString("zone", ZONE_LIST[menuZoneIndex]);
            tft.fillScreen(TFT_GREEN);
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.println("ZONE Set to:");
            tft.println(ZONE_LIST[menuZoneIndex]);
            delay(1500);
            esp_restart();
            confirmed = true;
        }
        delay(50);
    }
}