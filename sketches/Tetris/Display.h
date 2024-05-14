#include <LiquidCrystal.h> // Include the LiquidCrystal library for interfacing with LCD
#include <SPI.h>           // Include the SPI library
#define SS_PIN 10          // Define the Slave Select pin for SPI communication
#define DP_HEIGHT 16       // Define the height of the display area
#define DP_WIDTH 10        // Define the width of the display area

// Initialize the LCD object with pin configurations
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Function prototype for drawing a line on the display
void Dp_DrawLine(int x, int y, int length, bool hori, bool isSet);

// Define a structure representing a character area with a grid of 8x8
struct CharacterArea {
  byte grid[8] = {       // Grid representing the character area
    B00000,              // Each byte represents a row of pixels, where 1 indicates an active pixel and 0 indicates an inactive pixel
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };
};

CharacterArea sign[2][2]; // Array to hold character areas

// Function prototypes
//void Dp_Init();            // Initialize display
//void Dp_Draw();            // Draw function to refresh the display
//void Dp_SetPixel(int x, int y, bool isOn); // Function to set a pixel on the display
//bool Dp_GetPixel(int x, int y);            // Function to get the state of a pixel on the display

// Initialize display
void Dp_Init() {
  // Create custom characters
  lcd.createChar(0, sign[0][0].grid); // Create character 0 with sign[0][0].grid
  lcd.createChar(1, sign[0][1].grid); // Create character 1 with sign[0][1].grid
  lcd.createChar(2, sign[1][0].grid); // Create character 2 with sign[1][0].grid
  lcd.createChar(3, sign[1][1].grid); // Create character 3 with sign[1][1].grid

  // Draw border lines
  Dp_DrawLine(0, 0, 10, true, true);  // Top Line H
  Dp_DrawLine(0, 15, 10, true, true); // Bottom Line H
  Dp_DrawLine(0, 0, 16, false, true); // Left Line V
  Dp_DrawLine(9, 0, 16, false, true); // Right Line V
}

// Draw function to refresh the display
void Dp_Draw() {
  // Re-create custom characters
  lcd.createChar(0, sign[0][0].grid); // Recreate character 0
  lcd.createChar(1, sign[0][1].grid); // Recreate character 1
  lcd.createChar(2, sign[1][0].grid); // Recreate character 2
  lcd.createChar(3, sign[1][1].grid); // Recreate character 3

  // Draw characters on LCD
  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  lcd.setCursor(0, 1);
  lcd.write(byte(1));

  lcd.setCursor(1, 0);
  lcd.write(byte(2));

  lcd.setCursor(1, 1);
  lcd.write(byte(3));
}

// Function to set a pixel on the display
void Dp_SetPixel(int x, int y, bool isOn) {
  if (isOn)
    bitSet(sign[x / 5][y / 8].grid[y % 8], 4 - x % 5);
  else
    bitClear(sign[x / 5][y / 8].grid[y % 8], 4 - x % 5);
}

// Function to get the state of a pixel on the display
bool Dp_GetPixel(int x, int y) {
  return bitRead(sign[x / 5][y / 8].grid[y % 8], 4 - x % 5) == 1 ? true : false;
}

// Function to draw a line on the display
void Dp_DrawLine(int x, int y, int length, bool hori, bool isSet) {
  for (int offset = 0; offset < length; offset++) {
    if (hori)
      Dp_SetPixel(x + offset, y, isSet); // Set pixels horizontally
    else
      Dp_SetPixel(x, y + offset, isSet); // Set pixels vertically
  }
}
