/*
 * This file is part of Connect4 Game Solver <http://connect4.gamesolver.org>
 * Copyright (C) 2007 Pascal Pons <contact@gamesolver.org>
 *
 * Connect4 Game Solver is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Connect4 Game Solver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Connect4 Game Solver. If not, see <http://www.gnu.org/licenses/>.
 */

#include "solver.hpp"
#include <iostream>

#include <vector>
#include <algorithm>
#include <string>

#include <fstream>

bool compare(int a, int b);
std::vector<int> readLUT(std::string move_to_find); // Finds a given move 
std::string readLUT_compareBitmap(std::string bitMaptoCompare, int LUT, std::string current_move); // Compares all currently found Bitmaps - returns known vector if 

using namespace GameSolver::Connect4;

/*
 * Main function.
 * Reads Connect 4 positions, line by line, from standard input
 * and writes one line per position to standard output containing:
 *  - score of the position
 *  - number of nodes explored
 *  - time spent in microsecond to solve the position.
 *
 *  Any invalid position (invalid sequence of move, or already won game)
 *  will generate an error message to standard error and an empty line to standard output.
 */
int main(int argc, char** argv) {
  
  
  std::cout << "--- DO NOT SWITCH COMPUTER OFF ---\n" << std::endl;
	std::cout << "This Computer is being used to compute a look up table for a Summer Internship Project with Dom Jones" << std::endl;
	std::cout << "I am setting this off to compute over the weekend. I will be back on Monday the 5th of August to collect the results" << std::endl;
	std::cout << "\nIf it says that the code is finished and you need to use this computer, please contact me:" << std::endl;

	std::cout << "\nName: Jonathan Calladine" << std::endl;
	std::cout << "Email: mc20jmtc@leeds.ac.uk" << std::endl;

	std::cout << "\nCheers :)\n\n" << std::endl;

  // ---------------------------------------------------LUT 5---------------------------------------------------

  // Create and open a text file
  std::ofstream MyFile;
  //MyFile.open("/home/dvrkteam/Connect4Robot_Project/LUT/LUT_6.txt",std::ios::app); // std::ios::app - non destrictive writing [app = append mode]
  MyFile.open("/home/project/projects/Solver_Pascal/LUT/LUT_6.txt",std::ios::app); // std::ios::app - non destrictive writing [app = append mode]

  Solver solver; // Create an object
  bool weak = false; // select weak or strong solver

  std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings
  std::vector<int> colScores = {0,0,0,0,0,0,0}; // Vector containing the column scores

  int transpose_flip[7] = {6,4,2,0,-2,-4,-6}; // transposition matrix to flip the bitmap board

  // Find the score for each move in each column
  for (int i_1 = 0; i_1 < 7; i_1++) {
    for (int i_2 = 0; i_2 < 7; i_2++) {
      for (int i_3 = 0; i_3 < 7; i_3++) {
        for (int i_4 = 0; i_4 < 7; i_4++) {
          for (int i_5 = 0; i_5 < 7; i_5++) {
              for (int i_6 = 0; i_6 < 7; i_6++) {

              std::string move = colNumbers[i_1] + colNumbers[i_2] + colNumbers[i_3] + colNumbers[i_4] + colNumbers[i_5] + colNumbers[i_6]; // Create the move
              int int_move = ((i_1 + 1)*100000) + ((i_2 + 1)*10000) + ((i_3 + 1)*1000) + ((i_4 + 1)*100) + ((i_5 + 1)*10) + (i_6 + 1);
            
              if (int_move > 121211) {
                std::cout << move << std::endl; // Display current move

                int col_height[7] = {0,0,0,0,0,0,0};
                int bitmap[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                int flipped_bitmap[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

                // I_X = column to play in (index 0)
                // Col_height keeps track of the height of that col
                bitmap[i_1 + (7 * col_height[i_1])] = 1; // Add 1 to 
                col_height[i_1] = col_height[i_1] + 1; // Increment col height
                bitmap[i_2 + (7 * col_height[i_2])] = 2; // Add 2 to 
                col_height[i_2] = col_height[i_2] + 1; // Increment col height
                bitmap[i_3 + (7 * col_height[i_3])] = 1; // Add 1 to 
                col_height[i_3] = col_height[i_3] + 1; // Increment col height
                bitmap[i_4 + (7 * col_height[i_4])] = 2; // Add 2 to 
                col_height[i_4] = col_height[i_4] + 1; // Increment col height
                bitmap[i_5 + (7 * col_height[i_5])] = 1; // Add 1 to 
                col_height[i_5] = col_height[i_5] + 1; // Increment col height
                bitmap[i_6 + (7 * col_height[i_6])] = 2; // Add 2 to 
                col_height[i_6] = col_height[i_6] + 1; // Increment col height

                // Flip the bitmap
                for (int i = 0; i < 6; i++) {
                  for (int j = 0; j < 7; j++) {
                    flipped_bitmap[j + (i*7)] = bitmap[(j + (i*7)) + transpose_flip[j]];
                  }
                }          
                
                // Convert arrays to strings
                std::string bitmap_srt,flipped_bitmap_srt;
                for (int j = 0; j < 42; j++) {
                  bitmap_srt = bitmap_srt + std::to_string(bitmap[j]);
                  flipped_bitmap_srt = flipped_bitmap_srt + std::to_string(flipped_bitmap[j]);
                }

                // Compare the bitmaps
                std::string matching_move, matching_Flipped_move;
                matching_move = readLUT_compareBitmap(bitmap_srt,6,move);// Compare normal bitmap - find a matching move
                matching_Flipped_move = readLUT_compareBitmap(flipped_bitmap_srt,6,move); // Compare flipped bitmap - find a flipped matching move

                if (matching_move != "X") {
                  colScores = readLUT(matching_move);
                }
                else if (matching_Flipped_move != "X") {
                  colScores = readLUT(matching_Flipped_move);

                  std::vector<int> flippedScore = {0,0,0,0,0,0,0};
                  int j = 6;

                  // Flip the score
                  for (int i = 0; i < 7; i++) {
                    flippedScore[i] = colScores[j];
                    j--;
                  }

                  // Write the flipped score into the colScores Vector
                  for (int i = 0; i < 7; i++) {
                    colScores[i] = flippedScore[i];
                  }
                }
                else {
                  // If no flipped move or matched move found, calculate score
                  solver.reset();

                  for (int i = 0; i < 7; i++) { // index each coloumn

                    // Solver does not accept full columns: therefore, set colScore to arbitary large -ve value if column is full
                    // Solver does not accept winning moves: therefore, set colScore to arbitary large +ve value if column will win the game
                    if (col_height[i] == 6) { // Check if column is full
                      colScores[i] = -77; // If col is full, set to arbritary large -ve value - wont count in rankings
                    }
                    else {
                      move = move + colNumbers[i]; // Add column number to test

                      Position P;

                      if(P.play(move) != move.size()) { // Error Check
                        //std::cerr << "Invalid move " << (P.nbMoves()+1) << " \"" << move << "\"" << std::endl;
                        colScores[i] = 77; // If move is invalid = winning move: set to arbritary large +ve value - highest score in rankings
                      }
                      else {
                        int score = solver.solve(P, weak); // Find the score of the new potential move
                        score = - score; // Complement the score !! OUTPUT OF SOLVER IS INVERTED FOR SOME REASON
                        colScores[i] = score;
                      }

                      move.pop_back(); // remove the last column number ready for the next column number to be tested
                    }  
                  }
                }

                // Write Score to file 
                MyFile << move << " " << bitmap_srt << " [";
                for (int i = 0; i < 6; i++) {
                  MyFile << colScores[i] << " ";
                }
                MyFile << colScores[6] << "]" << std::endl; 
              }
            }
          }
        }
      }
    }
  }

  MyFile.close(); // Close the file 

	std::cout << "\nLUT 6 Finished Computing" << std::endl;

  /*
  
  // Game version!

  system("clear"); // Clear the terminal

  Solver solver; // Create an object
  bool weak = false; // select weak or strong solver

  std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings
  std::vector<int> colScores = {0,0,0,0,0,0,0}; // Vector containing the column scores
  int colHeight[7] = {0,0,0,0,0,0,0}; // Stores the height of each column
  std::string move = ""; // String to store the move sequence
  std::string col; // Varibleto store the column jut played

  while (1) {

    bool validCol = false;
    while (validCol == false) {
      std::cout << "Enter a Column: ";
      std::cin >> col;
      int col_index = std::stoi(col) - 1; // Convert str to int and subtract one to index the column array

      if (col_index < 0 || col_index > 6) { // Check if the input is in range
        std::cout << "Not a Valid Input, Enter a value between 1 and 7" << std::endl;
      }
      else if (colHeight[col_index] == 6) { // Check if column is full
        std::cout << "Column is Full, please select a differnt column" << std::endl;
      }
      else {
        validCol = true;
        colHeight[col_index] = colHeight[col_index] + 1; // Increment the column height
      }
    }
    
    move = move + col; // Concatinate the Column number to add the move to the move array
    std::cout << "\nMove: " << move << std::endl; // Output the move

    if (move.length() < 6) {
      colScores = readLUT(move);
    }
    else {
      solver.reset();

      for (int i = 0; i < 7; i++) {

        // Solver does not accept full columns: therefore, set colScore to arbitary large -ve value if column is full
        // Solver does not accept winning moves: therefore, set colScore to arbitary large +ve value if column will win the game
        if (colHeight[i] == 6) { // Check if column is full
          colScores[i] = -77; // If col is full, set to arbritary large -ve value - wont count in rankings
        }
        else {
          move = move + colNumbers[i]; // Add column number to test

          Position P;

          if(P.play(move) != move.size()) { // Error Check
            //std::cerr << "Invalid move " << (P.nbMoves()+1) << " \"" << move << "\"" << std::endl;
            colScores[i] = 77; // If move is invalid = winning move: set to arbritary large +ve value - highest score in rankings
          }
          else {
            int score = solver.solve(P, weak); // Find the score of the new potential move
            score = - score; // Complement the score !! OUTPUT OF SOLVER IS INVERTED FOR SOME REASON
            colScores[i] = score;
          }

          move.pop_back(); // remove the last column number ready for the next column number to be tested
        }
        
      }
    }

    for (int i = 0; i < 7; i++) {
      std::cout << colScores[i] << " ";
    }
    std::cout << std::endl;
  }

  
  */
}

// Comparison function for sort
bool compare(int a, int b) {return a > b;}

// Read Look Up Table 
std::vector<int> readLUT(std::string move_to_find) {
  
  int LUT = move_to_find.length(); // Find number of moves - corresponds to LUT identifier

  std::string LUT_str = std::to_string(LUT); // Convert LUT variable to a string
  std::string filename = "/home/project/projects/Solver_Pascal/LUT/LUT_" + LUT_str + ".txt"; // Create file name to index correct LUT
  std::ifstream LUT_file(filename); // Read from the text file

  std::string readLine;// Create a string for output of the file
  std::vector<int> scores = {0,0,0,0,0,0,0}; // scores vector

  // Read function line by line
  while (getline (LUT_file, readLine)) { // readLine = line variable

    std::string moveOnline = readLine.substr(0,LUT); // Finds move on the current line being serached

    if (moveOnline == move_to_find) { // Find the move
      int i = 0; // Index for element reading the line  
      int score = 0;
      int offset = offset = LUT + 1 + 42 + 1 + 1; // offset to read scores offset = (LUT = move.length()) + space + bitmap + space + bracket;

      // Find the scores in that line 
      for (int j = 0; j < 7; j++) {

        // Flags
        bool negativeFlag = false;

        // Check if element is negative
        if (readLine[offset + i] == 45) { // 45 = '-' in ASCII
          negativeFlag = true; // set flag true
          i++; // focus on next element
        }
        
        // Find the digits as ints (- 48 to convert char to int (48 = 0, 49 = 1, ...))
        int digit_1 = (readLine[offset + i] - 48);
        int digit_2 = (readLine[offset + i + 1] - 48);

        // Check if element is two digits - if element after == number: element is 2 digits
        if (digit_2 >= 0 && digit_2 <= 9) {
          score = (digit_1* 10) + digit_2; // score is two digits
          i = i + 3; // Move index 3 spaces over - 2 for the digits, 1 for the space
        }
        else {
          score = digit_1;
          i = i + 2; // Move index 2 spaces over - 1 for the digits, 1 for the space
        }

        if (negativeFlag == true) {score = - score;} // negate the score if minus is found

        scores[j] = score; // write the score to the scores vector
      }

      break; // Stop the while loop as matching move has been found
    }  
  }
  // Close the file
  LUT_file.close(); 
  return scores; // Return the scores vector
}

// Read Look Up Table and Compare bitmaps - return equivalent move 
std::string readLUT_compareBitmap(std::string bitMaptoCompare, int LUT, std::string current_move) {
  
  // Access the relevent LUT
  std::string LUT_str = std::to_string(LUT); // Convert LUT variable to a string
  std::string filename = "/home/project/projects/Solver_Pascal/LUT/LUT_" + LUT_str + ".txt"; // Create file name to index correct LUT
  std::ifstream LUT_file(filename); // Read from the text file

  std::string readLine;// Create a string for output of the file
  std::string bitmapInFile;// Create a string for output of the file
  std::string moveOutput = "X"; // Create a string for the move output - init as X, stays as X if no move detected
  
  // Read function line by line
  while (getline (LUT_file, readLine)) {
    bitmapInFile = readLine.substr((LUT+1),42); // Get the bitmap on a line

    if (bitmapInFile == bitMaptoCompare) { // Compare the bitmaps to check if any match
      moveOutput = readLine.substr((0),LUT); // Get the bitmap on a line
      //std::cout << moveOutput << std::endl;
      break;
    }

    // stop when current move found
    if (current_move == readLine.substr((0),LUT)) {
      break;
    } 
  }

  LUT_file.close(); // Close the file

  return moveOutput; // Return the move that matches
}
