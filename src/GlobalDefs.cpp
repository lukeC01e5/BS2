#include "GlobalDefs.h"

// Provide a single definition here
TFT_eSPI tft = TFT_eSPI();

// Also define these if needed:
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
bool allChallBools = false;

// RFIDData pendingData;
bool dataPending = false;
bool formSubmitted = false;

// Other global definitions...