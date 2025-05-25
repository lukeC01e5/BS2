#include "GlobalDefs.h"
#include <MFRC522.h>

// Define mfrc522 with its pins (SS_PIN and RST_PIN must be defined)
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the global key variable
MFRC522::MIFARE_Key key;

bool allChallBools = false; // Initialize as needed

//RFIDData pendingData;
bool dataPending = false;
bool formSubmitted = false;

// Other global definitions...