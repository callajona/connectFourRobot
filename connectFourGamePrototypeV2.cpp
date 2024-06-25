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

// Generate Empty Grid for Game
int grid [6][7] = {
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0}
};

// Global variable for the Position of the most recently placed counter
int counter_x;
int counter_y;

int main() {
  cout << " Connect Four!\n";
  displayGame();

  bool toggle = 1; // declare toggle variable (Toggles between 0 and 1)
  int player; // declare player variable (Toggles between 1 and 2)
  bool winCondition = false;

  while (winCondition == false) {
    toggle ^= 1; // Toggle varible to change player
    player = toggle + 1; // Add one to toggle to get player number
    placeCounter(player);

    displayGame(); // Display game

    // Check if recently placed counter wins game
    int win = winCheck(player);
    if (win != 0) {
      winCondition = true; // Game won so stop loop
      cout << "Player " << win << " has won!\n\n";
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
  for (int i = 0; i < 7; i++) {
    cout << i << " ";
  }
  cout << "\n";
  cout << "-------------\n";
  
}
void displayGame() {
  displayColNumbers();
  displayGrid();
}

int selectCol() {
  // Prompt player to select the column to place a counter into 

  int column; // Column variable
  bool stopCheck = false; // Varible to continue loop until pass condition met

  while (stopCheck == false) { // loop until empty column selected 
    cout << "Enter a Column to place your Counter: ";
    cin >> column;

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

  return column;
}
void placeCounter(int player) {
  // start at bottom of column
  // if grid slot == 0 - place player number into slot
  // else decrement search value

  int column = selectCol(); // Get Column 
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

  // ------------------------------------ Operation Description ------------------------------------
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
        if (grid[y][x-i] == player) { // check if adjacent grid slot has the same player number
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
        if (grid[y][x+i] == player) {
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
        if (grid[y+i][x] == player) {
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
        if (grid[y-i][x-i] == player) {
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
        if (grid[y+i][x+i] == player) {
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
        if (grid[y+i][x-i] == player) {
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
        if (grid[y-i][x+i] == player) {
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
  return returnVal;
}
