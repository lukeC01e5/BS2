#include <Keypad.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>
#include "RFIDData.h"
#include "GlobalDefs.h"
#include "quizAnswers.h"

//////////////////////////
///////  tavern ///////
/////////////////////////

// Keypad configuration (4x1 keypad)
const byte ROWS = 4;
const byte COLS = 1;
char keys[ROWS][COLS] = {
    {'1'},
    {'2'},
    {'3'},
    {'4'}};
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

void quizDisplay(int codeValue, int questionNumber);
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

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Starting Setup ===");

    // Initialize TFT, etc...
    tft.init();
    tft.setRotation(3);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("TFT Initialized");

    // Start SPI
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // This explicitly sets the default factory key (0xFF) for all 6 bytes
    for (int i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    // Initialize MFRC522
    mfrc522.PCD_Init();

    delay(1000); // Give the MFRC522 some time to settle

    // Wait for user to press keypad before continuing
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Press any key to start...");
    while (keypad.getKey() == NO_KEY)
    {
        delay(100);
    }
    tft.fillScreen(TFT_BLACK);
    Serial.println("=== Setup Complete ===\n");
}

void loop()
{
    // Create a UserTag object
    UserTag userTag;

    // Prompt user to present RFID
    tft.println("Present RFID Tag...");
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

    // Look up all four values for this challengeCode
    String val1, val2, val3, val4;
    if (player.challengeCode >= 0 && player.challengeCode < 10)
    {
        val1 = quizAnswers[player.challengeCode].value1;
        val2 = quizAnswers[player.challengeCode].value2;
        val3 = quizAnswers[player.challengeCode].value3;
        val4 = quizAnswers[player.challengeCode].value4;
    }
    else
    {
        val1 = "(Out of Range)";
        val2 = "(Out of Range)";
        val3 = "(Out of Range)";
        val4 = "(Out of Range)";
    }

    // Put all 4 question codes into an array so each can be asked in sequence
    String questionVals[4] = {val1, val2, val3, val4};

    // Keep track of how many times the user answered incorrectly
    int totalWrongAttempts = 0;

    // Ask each question in turn
    for (int qIndex = 0; qIndex < 4; qIndex++)
    {
        bool doneWithThisQuestion = false;
        while (!doneWithThisQuestion)
        {
            // Now pass questionVals[qIndex] and the questionNumber
            int thisCode = questionVals[qIndex].toInt();
            Serial.println("[DEBUG] Asking question # " + String(qIndex + 1) + " with codeValue=" + String(thisCode));
            quizDisplay(thisCode, qIndex + 1);

            // Wait for key press
            while (true)
            {
                char k = keypad.getKey();
                if (k == NO_KEY)
                {
                    delay(50);
                    continue;
                }
                Serial.println("[DEBUG] User pressed key: " + String(k));

                if (k == '1')
                {
                    tft.fillScreen(TFT_BLACK);
                    tft.setCursor(0, 0);
                    tft.setTextColor(TFT_GREEN, TFT_BLACK);
                    tft.println("Correct!");
                    Serial.println("[DEBUG] User answered question # " + String(qIndex + 1) + " correctly.");
                    doneWithThisQuestion = true;
                }
                else if (k == '2' || k == '3' || k == '4')
                {
                    totalWrongAttempts++;
                    tft.fillScreen(TFT_BLACK);
                    tft.setCursor(0, 0);
                    tft.setTextColor(TFT_RED, TFT_BLACK);
                    tft.println("Try again!");
                    Serial.println("[DEBUG] Wrong answer. totalWrongAttempts=" + String(totalWrongAttempts));
                    delay(1000);
                }
                break; // break inner while to re-ask or proceed
            }
        }
    }

    // After 4 correct answers, update player fields
    // Try posting 5 coin to DB
    bool postWorked = postToDBAdd5Coin();

    // Only update player.boolVal and write to RFID if postWorked
    if (postWorked)
    {
        // If DB post succeeded, set boolVal to 0 (which will appear as "00" in the tag)
        player.boolVal = 0;

        // Adjust wrongGuesses & build newBlockData as usual
        player.wrongGuesses -= totalWrongAttempts;
        if (player.wrongGuesses < 0)
            player.wrongGuesses = 0;
        char wgChar = (player.wrongGuesses < 10) ? (char)('0' + player.wrongGuesses) : '9';
        String newBlockData = formatCode(player.challengeCode) + wgChar + "?" +
                              String(player.boolVal) + "%" + player.playerName;

        // Show original and new data on Serial/TFT
        Serial.println("[DEBUG] Original Tag: " + userTag.contents);
        Serial.println("[DEBUG] New Tag    : " + newBlockData);
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.println("Original: " + userTag.contents);
        tft.println("New: " + newBlockData);

        // Use directWriteRFID to write to RFID
        if (directWriteRFID(mfrc522, key, newBlockData, 1))
        {
            Serial.println("[DEBUG] Block write success using directWriteRFID!");
        }
        else
        {
            Serial.println("[DEBUG] Block write failed using directWriteRFID!");
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
    Serial.println("=== End Loop, re-starting ===");
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
void quizDisplay(int codeValue, int questionNumber)
{
    Serial.println("[DEBUG] quizDisplay() called for codeValue=" + String(codeValue) + ", questionNumber=" + String(questionNumber));

    // 1) Clear screen & show heading
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Question " + String(questionNumber) + " :");

    // 2) Draw horizontal line about 1/5 from top
    int lineY = tft.height() / 5;
    tft.drawLine(0, lineY, tft.width(), lineY, TFT_WHITE);

    // 3) Prepare four boxes in bottom 4/5
    int regionH = tft.height() - lineY;
    int regionW = tft.width();
    int boxW = regionW / 2;
    int boxH = regionH / 2;

    // 4) Four colors
    uint16_t boxColors[4] = {TFT_RED, TFT_YELLOW, TFT_BLUE, TFT_GREEN};

    // 5) Create a range around codeValue for random distractors
    int minRange, maxRange;
    if (codeValue < 10)
    {
        minRange = 0;
        maxRange = 9;
    }
    else
    {
        minRange = (codeValue - 5 < 0) ? 0 : (codeValue - 5);
        maxRange = codeValue + 5;
    }

    const int totalBoxes = 4;
    int codeArray[totalBoxes];
    codeArray[0] = codeValue; // first entry is correct
    for (int i = 1; i < totalBoxes; i++)
    {
        codeArray[i] = randomInRangeExclude(minRange, maxRange, codeValue);
        for (int j = 0; j < i; j++)
        {
            while (codeArray[i] == codeArray[j])
            {
                codeArray[i] = randomInRangeExclude(minRange, maxRange, codeValue);
            }
        }
    }

    // 6) Draw boxes
    for (int i = 0; i < totalBoxes; i++)
    {
        int x = (i % 2) * boxW;
        int y = lineY + (i / 2) * boxH;
        tft.fillRect(x, y, boxW, boxH, boxColors[i]);
        tft.setTextColor(TFT_BLACK, boxColors[i]);
        tft.setCursor(x + 10, y + boxH / 2);
        tft.print(formatCode(codeArray[i]));
    }
    Serial.println("[DEBUG] quizDisplay() done drawing boxes.");
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
