#include <Keypad.h>
#include <Arduino.h>
#include <SPI.h>
//#include "RFIDData.h"
#include "GlobalDefs.h"
#include "quizAnswers.h"
#include "quizQuestions.h"
#include "animals.h"
#include <esp_system.h>  // Add this
#include "displayFunctions.h"

#define ZONE "water"

// Keypad configuration (4x1 keypad)
const byte ROWS = 4;
const byte COLS = 1;
char keys[ROWS][COLS] = {
    {'1'},    //red
    {'2'},    //green
    {'3'},    //blue
    {'4'}};   //yellow;

byte rowPins[ROWS] = {21, 17, 2, 15};
byte colPins[COLS] = {12};
Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// TFT and RFID objects
TFT_eSPI tft;
// MFRC522 mfrc522(SS_PIN, RST_PIN);
// MFRC522::MIFARE_Key key;

// Add a struct to hold the original tag data
struct UserTag
{
    String contents;
};

// Forward declarations
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

// Placeholder function to simulate "add_5_coin" post to DB
bool postToDBAdd5Coin()
{
    // TODO: Replace with real logic to post 5 coin to DB.
    // Return true if successful, false otherwise.
    Serial.println("[DEBUG] Simulating postToDBAdd5Coin...");
    // For now, just simulate success:
    return true;
}

// Add a small helper function to pick a random creature from ANIMALS[]
// based on ID and environment Zone:
AnimalInfo pickRandomCreature(int desiredId, const char *zone)
{
    // Build a list of matching creatures
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

    // If none match, return a default
    if (count == 0)
    {
        return {"NO_CREATURE", 1, "normal", "NC"};
    }

    // Otherwise pick a random one from the filtered list
    int index = random(0, count);
    return candidates[index];
}

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Starting Setup ===");

    // Initialize TFT, etc...
    tft.init();
    tft.setRotation(1);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    // tft.println("TFT Initialized");

    // Start SPI
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // Set factory key bytes to 0xFF
    for (int i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    // Initialize MFRC522
    mfrc522.PCD_Init();
    delay(1000); // Some settling time

    // Comment out or remove the keypad-wait:
    // tft.fillScreen(TFT_BLACK);
    // tft.setCursor(0, 0);
    // tft.setTextColor(TFT_GREEN, TFT_BLACK);
    // tft.println("Press any key to start...");
    // while (keypad.getKey() == NO_KEY)
    // {
    //     delay(100);
    // }
    // tft.fillScreen(TFT_BLACK);

    Serial.println("=== Setup Complete ===\n");
}

// The code now proceeds directly to loop() and waits for an RFID tag.
void loop()
{
    // Create a UserTag object
    UserTag userTag;

    // Prompt user to present RFID
    tft.fillScreen(TFT_BLACK);
    tft.println("Insert Key");
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        delay(100);
    }
    tft.println("Tag present!");

    // Authenticate with the default key (0xFF) for block 3
    byte trailerBlock = 3;
    MFRC522::StatusCode authStatus = mfrc522.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        trailerBlock,
        &key,
        &(mfrc522.uid));
    if (authStatus != MFRC522::STATUS_OK)
    {
        Serial.println("Auth failed: " + String(mfrc522.GetStatusCodeName(authStatus)));
        tft.println("Auth Failed");
        delay(2000);
        return;
    }

    // Read block 1 (where we store player data)
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

    // Convert block bytes to string
    String block1Data;
    for (int i = 0; i < 16; i++)
    {
        if (buffer[i] == 0)
            break;
        block1Data += (char)buffer[i];
    }

    // After reading block1Data successfully:
    userTag.contents = block1Data;

    // Parse block1Data into Player struct
    Player player;
    if (block1Data.length() < 4)
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
            player.boolVal = block1Data.substring(qIndex + 1, pIndex).toInt();
            player.playerName = block1Data.substring(pIndex + 1);
        }
    }

    // Look up all three values for this challengeCode
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

    // Put all 3 question codes into an array so each can be asked in sequence
    String questionVals[3] = {val1, val2, val3};

    // Keep track of how many times the user answered incorrectly
    int totalWrongAttempts = 0;

    // Ask each question in turn
    for (int qIndex = 0; qIndex < 3; qIndex++)
    {
        bool doneWithThisQuestion = false;
        while (!doneWithThisQuestion)
        {
            int thisCode = questionVals[qIndex].toInt();
            Serial.println("[DEBUG] Asking question # " + String(qIndex + 1) +
                           " with codeValue=" + String(thisCode));

            // quizDisplay returns the index (0..3) of the correct box
            int correctIndex = quizDisplay(thisCode, qIndex + 1);

            // Wait for key press
            while (true)
            {
                char k = keypad.getKey();
                if (k == NO_KEY)
                {
                    delay(50);
                    continue; // wait for a key again
                }
                Serial.println("[DEBUG] User pressed key: " + String(k));

                // Map '1','2','3','4' to box indices 0..3
                int pressedBox = -1;
                if (k == '1')
                    pressedBox = 0; // red
                else if (k == '2')
                    pressedBox = 1; // yellow
                else if (k == '3')
                    pressedBox = 2; // blue
                else if (k == '4')
                    pressedBox = 3; // green

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
                    doneWithThisQuestion = true; // exit while
                }
                else
                {
                    // Wrong!
                    totalWrongAttempts++;
                    tft.fillScreen(TFT_BLACK);
                    tft.setCursor(0, 0);
                    tft.setTextColor(TFT_RED, TFT_BLACK);
                    tft.println("Try again!");
                    Serial.println("[DEBUG] Wrong answer. totalWrongAttempts=" +
                                   String(totalWrongAttempts));
                    delay(1000);
                }
                break; // break inner while to re-ask or proceed
            }
        }
    }

    // After 3 correct answers, update player fields
    // Try posting 5 coin to DB
    bool postWorked = postToDBAdd5Coin();

    // Only update player.boolVal and write to RFID if postWorked
    if (postWorked)
    {
        // Set boolVal to 15
        player.boolVal = 15;

        // Randomly pick a value 0..30
        int randomVal = random(0, 31);
        String creatureCode = "";
        String creatureName = "";

        if (randomVal <= 20)
        {
            // No creature acquired
            tft.fillScreen(TFT_RED);
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_BLACK, TFT_RED);
            tft.println("No creature acquired");
        }
        else if (randomVal <= 27)
        {
            // Pick an ID=2 creature for the defined ZONE
            AnimalInfo chosen = pickRandomCreature(2, ZONE);
            creatureCode = chosen.code;
            creatureName = chosen.name;

            tft.fillScreen(TFT_GREEN);
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.println("Creature Acquired: " + String(creatureName));
        }
        else // randomVal between 28 and 30
        {
            // Pick an ID=3 creature for the defined ZONE
            AnimalInfo chosen = pickRandomCreature(3, ZONE);
            creatureCode = chosen.code;
            creatureName = chosen.name;

            tft.fillScreen(TFT_GREEN);
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.println("Creature Acquired: " + String(creatureName));
        }

        // Decrease wrongGuesses by the totalWrongAttempts
        player.wrongGuesses -= totalWrongAttempts;
        if (player.wrongGuesses < 0)
            player.wrongGuesses = 0;
        char wgChar = (player.wrongGuesses < 10) ? (char)('0' + player.wrongGuesses) : '9';

        // Build newBlockData. If no creature is acquired, creatureCode stays empty
        // For example: "1239?15%BR000"
        String newBlockData = formatCode(player.challengeCode) + wgChar +
                              "?" + String(player.boolVal) + "%" +
                              creatureCode;

        Serial.println("[DEBUG] Original Tag: " + userTag.contents);
        Serial.println("[DEBUG] New Tag    : " + newBlockData);

        // Write to RFID
        if (directWriteRFID(mfrc522, key, newBlockData, 1))
        {
            Serial.println("[DEBUG] Block write success with new code!");
        }
        else
        {
            Serial.println("[DEBUG] Block write failed!");
        }
    }
    else
    {
        // If the DB post fails, skip writing to RFID
        Serial.println("[DEBUG] postWorked == false; skipping RFID write...");
    }

    // Now halt the card
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    Serial.println("[DEBUG] Halted and stopped crypto after write.");

    delay(3000);
    tft.fillScreen(TFT_BLACK);
    Serial.println("=== End Loop, rebooting now ===");

    // Reboot the microcontroller
    esp_restart();
}

/// Helper to produce a 3-digit string (e.g. "005", "184") for display
String formatCode(int codeValue)
{
    if (codeValue < 0)
        codeValue = 0; // limit negative range
    char buf[5];
    snprintf(buf, sizeof(buf), "%03d", codeValue);
    return String(buf);
}

/// Return random int in [minRange..maxRange], excluding excludeVal
int randomInRangeExclude(int minRange, int maxRange, int excludeVal)
{
    if (maxRange < minRange)
        return excludeVal; // safety check
    int val;
    do
    {
        val = random(minRange, maxRange + 1);
    } while (val == excludeVal);
    return val;
}

String buildQuestionPrompt(int codeValue, int questionNumber)
{
    if (codeValue >= 0 && codeValue <= 20)
    {
        // questionNumber will be 1, 2, or 3 during your loop
        int qIndex = questionNumber - 1; // convert to 0..2
        if (qIndex < 0 || qIndex > 2)
            qIndex = 0; // safety clamp

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
    // Fallback if out of range
    return "Question " + String(questionNumber) + ":";
}

/**
 * Draws a question screen on the TFT:
 *  - Clears display
 *  - Prints "Question X :"
 *  - Draws horizontal line ~1/5 from the top
 *  - Creates 4 colored boxes (Red, Yellow, Blue, Green)
 *  - Places codeValue in black text in one of the boxes, the others have random codes
 *
 *  param codeValue: the “correct” integer code
 *  param questionNumber: the question number to display
 */
//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\main2.cpp
int quizDisplay(int codeValue, int questionNumber)
{
    Serial.println("[DEBUG] quizDisplay() for codeValue=" + String(codeValue) +
                   ", questionNumber=" + String(questionNumber));

    // Clear screen & set text
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Display question prompt
    String questionText = buildQuestionPrompt(codeValue, questionNumber);
    tft.println(questionText);

    // Horizontal line
    int lineY = tft.height() / 5;
    tft.drawLine(0, lineY, tft.width(), lineY, TFT_WHITE);

    // Figure out which quizAnswers value is correct for this questionNumber
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
        correctAnswer = "0"; // fallback or placeholder
    }

    // Create array of 4 answers: 1 correct + 3 distractors
    String answerOptions[4];
    answerOptions[0] = correctAnswer; // put correct first

    // Get 3 random distractors that are *not* the same as correctAnswer
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

            // Only break out if distractor != correctAnswer
            if (distractor != correctAnswer)
                break;
        }
        answerOptions[i] = distractor;
    }

    // Then shuffle the array so the correct answer isn't always at index 0
    for (int i = 0; i < 4; i++)
    {
        int r = random(i, 4);
        String temp = answerOptions[i];
        answerOptions[i] = answerOptions[r];
        answerOptions[r] = temp;
    }

    // Identify which box holds the correctAnswer
    int correctBoxIndex = 0;
    for (int i = 0; i < 4; i++)
    {
        if (answerOptions[i] == correctAnswer)
        {
            correctBoxIndex = i;
            break;
        }
    }

    // 4 colored boxes: 0=Red, 1=Yellow, 2=Blue, 3=Green
    const uint16_t boxColors[4] = {TFT_RED, TFT_YELLOW, TFT_BLUE, TFT_GREEN};

    // Layout: 2x2 grid in bottom 4/5 of screen
    int regionH = tft.height() - lineY;
    int regionW = tft.width();
    int boxW = regionW / 2;
    int boxH = regionH / 2;

    // Draw each box with the corresponding answer text
    // (index 0 => top-left (Red), 1 => top-right (Yellow), etc.)
    for (int i = 0; i < 4; i++)
    {
        int x = (i % 2) * boxW;
        int y = lineY + (i / 2) * boxH;
        tft.fillRect(x, y, boxW, boxH, boxColors[i]);
        tft.setTextColor(TFT_BLACK, boxColors[i]);
        tft.setCursor(x + 10, y + boxH / 2);
        tft.print(answerOptions[i]);
    }

    // Log which one is correct
    Serial.println("[DEBUG] correctAnswer=" + correctAnswer +
                   ", correctBoxIndex=" + String(correctBoxIndex));

    return correctBoxIndex;
}

/**
 * Writes data directly to an RFID block.
 *
 * param rfid: Reference to the MFRC522 object
 * param key: Reference to the MIFARE_Key object
 * param data: The data to write (String, max 16 bytes)
 * param blockNumber: The block number to write to
 * return: true if successful, false otherwise
 */
bool directWriteRFID(MFRC522 &rfid, MFRC522::MIFARE_Key &key, const String &data, byte blockNumber)
{
    // Authenticate for the target block
    byte trailerBlock = blockNumber + 3 - (blockNumber % 4); // typical for MIFARE Classic
    MFRC522::StatusCode status = rfid.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        trailerBlock,
        &key,
        &(rfid.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.println("Authentication failed: " + String(rfid.GetStatusCodeName(status)));
        return false;
    }

    // Prepare 16-byte array for the block write
    byte blockContent[16];
    memset(blockContent, 0, sizeof(blockContent));
    data.getBytes(blockContent, 16); // clip or pad to 16 bytes

    // Write to the block
    status = rfid.MIFARE_Write(blockNumber, blockContent, 16);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.println("Write failed: " + String(rfid.GetStatusCodeName(status)));
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        return false;
    }

    Serial.println("Write success for block " + String(blockNumber));

    // Halt & stop crypto
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return true;
}
