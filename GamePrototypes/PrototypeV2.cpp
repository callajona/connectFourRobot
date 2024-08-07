// --------------------------------------------------------------------------------
//  Connect Four Prototype
//
//    Author: Jonathan Calladine
//    Date Created: 24/06/2024
//    Last Modified: 25/06/2024
//
//  This code creates a game of Connect Four that can be played in the terminal
//  The code detects when a win has occured and stops 
//
//    Requires: "connectFourPrototype.h"
//
// --------------------------------------------------------------------------------

#include <iostream>
#include "connectFourPrototype.h"

using namespace std;

// Global variables
// for the Position of the most recently placed counter
int counter_x;
int counter_y;

int g_moveList[42]; // Stores list of moves
int g_turnCounter; // Keeps track of what move the game is on - index 0

// Generate Empty Grid for Game
int grid [6][7];

int main() {
  cout << " Connect Four!\n";
  displayGame();

  bool toggle = 1; // declare toggle variable (Toggles between 0 and 1)
  int player; // declare player variable (Toggles between 1 and 2)
  bool winCondition = false;

  int p1_score = 0;
  int p2_score = 0;

  while (winCondition == false) {
    toggle ^= 1; // Toggle varible to change player
    player = toggle + 1; // Add one to toggle to get player number
    placeCounter(player);

    displayGame(); // Display game

    // Check if recently placed counter wins game
    int win = winCheck(player);
    if (win != 0) {
      winCondition = true; // Game won so stop loop
      cout << "Player " << win << " has won!\n";
      
      // Move list display
      cout << "Move List: ";
      for (int i = 0; i <= g_turnCounter; i++) {
        cout << g_moveList[i];
      }
      
      // Score System
      if (player == 1) {
        p1_score++;
      }
      else if (player == 2) {
        p2_score++;
      }
      cout << "\n\n---SCORE---\n" << "Player 1: " << p1_score << "\nPlayer 2: " << p2_score << "\n\n";

      // Reset Game
      resetGrid();
      g_turnCounter = 0; // reset turn counter
      winCondition = false;
      cout << "Connect Four!\n";
      displayGame();
    }
    else {
      g_turnCounter++; // Increment turn counter
    }


  }
}

void displayGrid() {
  // Display each element in a 7x6 grid
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      cout << grid [i][j] << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}
void displayColNumbers() {
  // Display the column number above column
  cout << "\n";
  for (int i = 1; i <= 7; i++) {
    cout << i << " ";
  }
  cout << "\n";
  cout << "-------------\n";
  
}
void displayGame() {
  displayColNumbers();
  displayGrid();
}

int selectCol(int player) {
  // Prompt player to select the column to place a counter into 

  int column; // Column variable
  bool stopCheck = false; // Varible to continue loop until pass condition met

  while (stopCheck == false) { // loop until empty column selected 
    cout << "Player " << player << ", select a column: "; // Between 1 and 7
    cin >> column; 
    column--; // Decrease column value by 1 to index to 0 for the grid array

    if (grid[0][column] != 0) { // Check if column is full
      cout << "Column Full - Please select another column" << endl;
    }
    else if (column < 0 || column >= 7) { // Check if input is within acceptible range
        cout << "Invalid Column Number. Enter value between 1 and 7" << endl;
      }
    else {
      stopCheck = true; // Input valid - stop loop
    }
  }

  moveList(column+1); // Increase value by 1 to index to 1 (not index to 0)
  return column;
}
void moveList(int column) {
  g_moveList[g_turnCounter] = column; // Add recent move to variable
}
void placeCounter(int player) {
  // start at bottom of column
  // if grid slot == 0 - place player number into slot
  // else decrement search value

  int column = selectCol(player); // Get Column 
  counter_x = column; // Column = X coordinate of recently placed counter

  bool stopCheck = false; // false while lowest zero not found
  int rowIndex = 5; // Row serch value

  while(stopCheck == false) {
    if (grid[rowIndex][column] == 0) { // Search Column for 0
      grid[rowIndex][column] = player; // Set coordiate as player number if empty
      stopCheck = true; // Stop while loop
      counter_y = rowIndex; // Row = Y coordinate of recently placed counter
    }
    else {
      rowIndex--; // Decrement rowIndex to search next row up
    }
  }  
}

int winCheck(int player) {
  // Check for a win - return 0 for no win, return player number for win detected

  // -------------------------------- Operation Description ------------------------------------------------------
  // Centred around newest placed counter - Coordinates from global varibles
  // Check each Plane (Horizontal [L+R], vertical [down only], Diagonal [NW-SE], Diagonal [SW-NE])
  // Count no. of adjacent counters that match player number: in both directions [L+R, NW+SE, SW+NE]
  // If counters counter variable for given plane >= 4 --> Win
  // Else no win

  // Counters Counter Varibles
  int HCC = 1; // Horizontal Counters Counter
  int VCC = 1; // Vertia Counters Counter
  int DNWCC = 1; // Diagonal North-West Counters Counter
  int DSWCC = 1; // Diagonal South-West Counters Counter

  // Coordiantes
  int x = counter_x;
  int y = counter_y;

  int returnVal = 0; // value to be returned - remains 0 if no win found
  int searchIndex = 0; // Search index for the different directions - breaks while loop when win found
  int i;// Search Value for grid
  bool stopCheck; // Stop when win found, or no further counters to be checked

  while (searchIndex != 4) { // Loop until win found or no win found after all checks completed
    
    if (searchIndex == 0) {  // ------------- Horizontal ---------------------
      // Left Horizontal Check
      i = 1; // reset index
      stopCheck = false; // reset stopecheck
      while (stopCheck == false) { 
        if ((x-1) < 0) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y][x-i] == player) { // check if adjacent grid slot has the same player number
          HCC++; // increment counters counter
          i++; // increment search value
        }
        else {
          stopCheck = true; // stop while loop when opposite player number found
        }
      }

      // Right Horizontal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((x+1) > 6) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y][x+i] == player) {
          HCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }

      // Check if horizontal win
      if (HCC >= 4) {
        returnVal = player; // Declare player X to have won
        searchIndex = 4; // Stop search
      }
      else {
        searchIndex++;
      }
    }
      
    else if (searchIndex == 1) { // -------------- Vertical ---------------------- 
      // Down Vertical Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((y+i) > 5) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y+i][x] == player) {
          VCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
  
      // Check if Vertical win
      if (VCC >= 4) {
        returnVal = player; // Declare player X to have won
        searchIndex = 4; // Stop search
      }
      else {
        searchIndex++;
      }
    }

    else if (searchIndex == 2) { // -------------- NW Diagonal ----------------------

      // NW Diagonal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((y-i) < 0 || (x-1) < 0) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y-i][x-i] == player) {
          DNWCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
  
      // SE Diagonal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((y+i) > 5 || (x+1) > 6) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y+i][x+i] == player) {
          DNWCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
  
      // Check if NW Diagonal win
      if (DNWCC >= 4) {
        returnVal = player; // Declare player X to have won
        searchIndex = 4; // Stop search
      }
      else {
        searchIndex++;
      }
    }

    else if (searchIndex == 3) { // -------------- SW Diagonal ----------------------
      // SW Diagonal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((y+i) > 5 || (x-1) < 0) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y+i][x-i] == player) {
          DSWCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
    
      // NE Diagonal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if ((y-i) < 0 || (x+1) > 6) {
          // Only search within area of the board - could find a match with the other variables in the continous memory space otherwise
          stopCheck = true;
        }
        else if (grid[y-i][x+i] == player) {
          DSWCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
    
      // Check if SW Diagonal win
      if (DSWCC >= 4) {
        returnVal = player; // Declare player X to have won
        searchIndex = 4; // Stop search
      }
      else {
        searchIndex++;
      }
    }
  }

  cout << "\nH:" << HCC << " V:" << VCC << " DNW:" << DNWCC << " DSW:" <<  DSWCC << endl;
  return returnVal;
}

void resetGrid() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      grid[i][j] = 0;
    }
  }
}
