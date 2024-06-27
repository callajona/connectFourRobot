#include <iostream>

using namespace std;

// Function Prototypes
void displayGrid();
void displayColNumbers();
int selectCol();
void placeCounter(int player);

int winCheck(int player);

// Generate Grid for Game
// 0 = empty, 1 = red, 2 = yellow
int grid [6][7] = {
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0}
};

int counter_x;
int counter_y;

int main() {
  displayColNumbers();
  displayGrid();

  bool playerToggle = 1;
  int player;

  bool winCondition = false;

  while (winCondition == false) {
    playerToggle ^= 1;
    player = playerToggle + 1;
    placeCounter(player);

    // Display game
    displayColNumbers();
    displayGrid();

    // check if player that has just placed a counter has won
    int win = winCheck(player);
    if (win != 0) {
      winCondition = true;
      cout << "Player " << win << " has won!" << endl;
    }
  }
}

void displayGrid() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      cout << grid [i][j] << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

void displayColNumbers() {
  cout << "\n";
  for (int i = 0; i < 7; i++) {
    cout << i << " ";
  }
  cout << "\n";
  cout << "-------------\n";
  
}

int selectCol() {
  // Prompt user to select the column to place a counter into 

  int column; // Column variable
  bool passCondition = false; 

  // loop until empty column selected 
  while (passCondition == false) {
    cout << "Enter a Column to place your Counter: ";
    std::cin >> column;

    if (grid[0][column] != 0) {
      cout << "Column Full - Please select another column" << endl;
    }
    else if (column < 0 || column >= 7) {
        cout << "Invalid Column Number. Enter value between 1 and 7" << endl;
      }
    else {
      passCondition = true;
    }
  }

  return column;
}

void placeCounter(int player) {
  // start at bottom of column
  // if == 0 - place player number into slot
  // else decrement search value

  int column = selectCol();
  counter_x = column;

  bool test = false; // false while lowest zero not found
  int i = 5; // Row serch value

  while(test == false) {
    if (grid[i][column] == 0) {
      grid[i][column] = player;
      test = true;
      counter_y = i;
    }
    else {
      i--; // Decrement i to search next row up
    }
  }  
}

int winCheck(int player) {
  // Check for a win - return 0 for no win, return player number for win detected

  // look at new counter
  // horizontal counters number - HCN
  //  search if counter on LHS = same type - HCN + 1: Continue until counter on LHS not same type or HCN = 4
  //   search Right and continue to add to HCN 
  // repeat for vert

  int HCC = 1; // Horizontal Counters Counter
  int VCC = 1; // Vertia Counters Counter
  int DNWCC = 1; // Diagonal North-West Counters Counter
  int DSWCC = 1; // Diagonal South-West Counters Counter

  int x = counter_x;
  int y = counter_y;
  int returnVal = 0;
  int searchIndex = 0;

  int i;// Search Value
  bool stopCheck; // Stop when win found, or no further counters to be checked

  while (searchIndex != 4) {
    
    if (searchIndex == 0) {  // ------------- Horizontal ---------------------
      // Left Horizontal Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if (grid[y][x-i] == player) {
          HCC++;
          i++;
        }
        else {
          stopCheck = true;
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
      // Up Vertical Check
      i = 1;
      stopCheck = false;
      while (stopCheck == false) {
        if (grid[y-i][x] == player) {
          VCC++;
          i++;
        }
        else {
          stopCheck = true;
        }
      }
  
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
