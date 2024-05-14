// Include the header file "Block.h"
#include "Block.h"

// Define the pin numbers for the left, right, and rotation buttons
#define BUTTON_LEFT 13
#define BUTTON_RIGHT 12
#define BUTTON_ROT 11

// Define variables to store the previous state of each button
bool BUTTON_LEFT_LAST = false;
bool BUTTON_RIGHT_LAST = false;
bool BUTTON_ROT_LAST = false;

// Initialize a variable to store the player's points
long points = 0;

// Declare a variable to store the current block
Block currentBlock;

// Function to initialize the Tetris game
void Te_Init()
{
  // Initialize the current block
  currentBlock = Bl_InitBlock();
  // Set the pin modes for the buttons
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_ROT, INPUT);
}

// Function to draw the Tetris game
void Te_Draw()
{
  // Print the current block on the display
  Bl_Print(currentBlock, true);
  // Draw the display
  Dp_Draw();
  // Clear the current block from the display
  Bl_Print(currentBlock, false);
}

// Function to add points to the player's score
void Te_AddPoints(long p)
{
  // Increment the points by the provided value
  points += p;
  // Print the updated points on the LCD
  lcd.setCursor(10, 0);
  lcd.print(points);
}

// Function to delete a filled row from the display
void Te_DeleteRow(int row)
{
  bool sw = true;
  for (int i = 0; i < 15; i++)
  {
    Dp_DrawLine(1, row, 8, true, sw);
    Dp_DrawLine(0,0,10,true,!sw); // Top Line H
    Dp_DrawLine(0,15,10,true,!sw); // Bottom Line H
    Dp_DrawLine(0,0,16,false,!sw); // Left Line  V
    Dp_DrawLine(9,0,16,false,!sw); // Right Line V
    Dp_Draw();
    delay(90);
    sw = sw ? false : true;
  }
  Dp_DrawLine(1, row, 8, true, false);
  Dp_DrawLine(0,0,10,true,true); // Top Line H
  Dp_DrawLine(0,15,10,true,true); // Bottom Line H
  Dp_DrawLine(0,0,16,false,true); // Left Line V
  Dp_DrawLine(9,0,16,false,true); // Right Line V
  Dp_Draw();
  for (int line = row; line > 2; line--)
  {
    for (int x = 1; x < TE_WIDTH + 1; x++)
    {
      Dp_SetPixel(x, line, Dp_GetPixel(x, line - 1));
    }
  }
}

// Function to check for filled rows and delete them
void Te_CheckRows()
{
  byte rowWins = 0;
  for (int line = TE_HEIGHT; line> 0; line--)
  {
    bool lineFull = true;
    for (int x = 1; x < TE_WIDTH; x++)
    {
      if (!Dp_GetPixel(x, line)) // Checks if there is still a piece free in the current line
      {
        lineFull = false;
        break;
      }
    }
    if (lineFull) // Checks if the current line is full
    {
      rowWins++;
      Te_DeleteRow(line);
      line++;
    }
  }
  if (rowWins > 0)
    Te_AddPoints(random(9, 15) * (10 + pow(3,rowWins) * 10));
}

// Function to reload the Tetris game after a game over
void Te_Reload()
{
  for (int row = 1; row < TE_HEIGHT + 1; row++)
  {
    Dp_DrawLine(1, row, 8, true, true);
    Dp_Draw();
    delay(60);
  }
  delay(400);
  for (int row = TE_HEIGHT; row >= 1 ; row--)
  {
    Dp_DrawLine(1, row, 8, true, false);
    Dp_Draw();
    delay(60);
  }
  currentBlock = Bl_InitBlock();
  points = 0;
  lcd.setCursor(10, 0);
  lcd.print("0     ");
  lcd.setCursor(3, 1);
  lcd.print("             ");
}

// Function to update the Tetris game state
void Te_Update(long frameCount)
{
  if (frameCount % 40  == 0)
  {
    currentBlock.y++;
    if (Bl_HitTest(currentBlock)) //Checks if the block has reached the bottom
    {
      currentBlock.y--;
      for(int i = 0; i < 5; i++)
      {
        Bl_Print(currentBlock, true);
        Dp_Draw();
        delay(60);
        Bl_Print(currentBlock, false);
        Dp_Draw();
        delay(60);
      }
      Bl_Print(currentBlock, true);
      currentBlock = Bl_InitBlock();
      Dp_Draw();
      if (Bl_HitTest(currentBlock))
      {
        lcd.setCursor(3, 1);
        lcd.write("Game Over!");
        delay(1000);
        while (!digitalRead(BUTTON_LEFT) || !digitalRead(BUTTON_RIGHT));
        Te_Reload();
      }
      Te_AddPoints(random(1, 4) * 10);
      Te_CheckRows();
      return;
    }
  }


  // Check for player input to move the block left, right, or rotate
  if (digitalRead(BUTTON_LEFT))
  {
    if (!BUTTON_LEFT_LAST)
    {
      BUTTON_LEFT_LAST = true;

      // Tries to move the block to the left
      currentBlock.x--;
      if (Bl_HitTest(currentBlock))
        currentBlock.x++;
    }
  }
  else BUTTON_LEFT_LAST = false;

  if (digitalRead(BUTTON_RIGHT))
  {
    if (!BUTTON_RIGHT_LAST)
    {
      BUTTON_RIGHT_LAST = true;

      // Tries to move the block to the right 
      currentBlock.x++;
      if (Bl_HitTest(currentBlock))
        currentBlock.x--;
    }
  }
  else BUTTON_RIGHT_LAST = false;

  if (digitalRead(BUTTON_ROT))
{
  if (!BUTTON_ROT_LAST)
  {
    BUTTON_ROT_LAST = true;

    // Rotates the stone
    Bl_Rotate(&currentBlock);
    if (Bl_HitTest(currentBlock))
      Bl_BackRotate(&currentBlock);
  }
}
else BUTTON_ROT_LAST = false;

}

