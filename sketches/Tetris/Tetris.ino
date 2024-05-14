#include <LiquidCrystal.h>  // Include the LiquidCrystal library for interfacing with LCD
#include <SPI.h>            // Include the SPI library
#include "Tetris.h"         // Include the Tetris game library
#define SS_PIN 10           // Define the SS (Slave Select) pin

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);         // Initialize a 16x2 LCD
  lcd.print("   Score: 0"); // Print initial score message on the LCD
  Serial.begin(9600);       // Initialize serial communication at 9600 baud
  randomSeed(analogRead(0));// Seed the random number generator with an analog input value
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Loading ..."); // Print loading message on the serial monitor
  Dp_Init();            // Initialize display properties
  Te_Init();            // Initialize Tetris game
  // Print random numbers with some mathematical operations
  Serial.println(random(9, 15) * (10 + pow(3,1) * 10));
  Serial.println(random(9, 15) * (10 + pow(3,2) * 10));
  Serial.println(random(9, 15) * (10 + pow(3,3) * 10));
  Serial.println(random(9, 15) * (10 + pow(3,4) * 10));
}

byte xC = 0;      // Initialize x-coordinate for Tetris piece
byte yC = 0;      // Initialize y-coordinate for Tetris piece
long frameCount = 0;  // Initialize frame count for Tetris game

void loop() {
  frameCount++;       // Increment frame count
  Te_Draw();          // Draw Tetris game
  Te_Update(frameCount); // Update Tetris game based on frame count
  delay(1);           // Small delay to control the game speed
}
