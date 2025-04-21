#include <Keypad.h>
#include <TFT_eSPI.h> // Include the TFT library

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[ROWS] = {21, 17, 2, 15}; // Rows connected to ESP32
byte colPins[COLS] = {12, 27, 32};    // Columns connected to ESP32

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT display

void setup()
{
    Serial.begin(115200);
    tft.init();
    tft.setRotation(3);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("TFT Initialized");
}

void loop()
{
    char key = keypad.getKey();
    if (key)
    {
        Serial.println(key);
        tft.println(key); // Display the key on the TFT display
    }
}
