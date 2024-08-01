#include "solver.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <fstream>

bool compare(int a, int b);
std::vector<int> readLUT(std::string move_to_find); // Finds a given move 
std::string readLUT_compareBitmap(std::string bitMaptoCompare, int LUT); // Compares all currently found Bitmaps - returns known vector if 

using namespace GameSolver::Connect4;

int main(int argc, char** argv) {

  // Create and open a text file
  std::ofstream MyFile;
  MyFile.open("/home/project/projects/Solver_Pascal/LUT/LUT_4.txt",std::ios::app); // std::ios::app - non destrictive writing [app = append mode]

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

          std::string move = colNumbers[i_1] + colNumbers[i_2] + colNumbers[i_3] + colNumbers[i_4]; // Create the move
          int int_move = ((i_1 + 1)*1000) + ((i_2 + 1)*100) + ((i_3 + 1)*10) + (i_4 + 1);

          if (int_move > 2327) {
            std::cout << int_move << std::endl;

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
            matching_move = readLUT_compareBitmap(bitmap_srt, 4);// Compare normal bitmap - find a matching move
            matching_Flipped_move = readLUT_compareBitmap(flipped_bitmap_srt, 4); // Compare flipped bitmap - find a flipped matching move

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
              // If not already found, run the move
              solver.reset(); // Reset the solver
              for (int i = 0; i < 7; i++) {
                move = move + colNumbers[i]; // Add column number to test

                Position P;

                if(P.play(move) != move.size()) { // Error Check
                  std::cerr << "Invalid move " << (P.nbMoves()+1) << " \"" << move << "\"" << std::endl;
                }
                else {
                  int score = solver.solve(P, weak); // Find the score of the new potential move
                  score = - score; // Complement the score !! OUTPUT OF SOLVER IS INVERTED FOR SOME REASON
                  colScores.insert(colScores.begin() + i, score); // Add the scores to score vector
                }
                move.pop_back(); // remove the last column number ready for the next column number to be tested
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
  MyFile.close(); // Close the file 
}

// Comparison function for sort
bool compare(int a, int b) {return a > b;}

// Read Look Up Table 
std::vector<int> readLUT(std::string move_to_find) {
  
  int LUT = move_to_find.length(); // Find number of moves - corresponds to LUT identifier

  std::string LUT_str = std::to_string(LUT); // Convert LUT variable to a string
  std::string filename = "/home/project/projects/Solver_Pascal/LUT/LUT_" + LUT_str + ".txt"; // Create file name to index correct LUT
  std::ifstream LUT_file(filename); // Read from the text file

  std::string move_score;// Create a string for output of the file
  std::vector<int> scores = {0,0,0,0,0,0,0}; // scores vector

  // Read function line by line
  while (getline (LUT_file, move_score)) {

    std::string moveOnline = move_score.substr(0,LUT); // Finds move on the current line being serached

    if (moveOnline == move_to_find) { // Find the move
      int i = 0; // Index for element in the line  
      int offset = offset = LUT + 1 + 42 + 1 + 1; // LUT (=move.length()) + space + bitmap + space + bracket;
      
      // Find the scores in that line 
      for (int j = 0; j < 7; j++) {
        if (move_score[i + offset] == '-') {
          i++; // Increment i to find number next to the minus symbol
          scores[j] = 0 - (move_score[i + offset] - 48); // Negate the score found (- 48 as 0-9 digits start from 48 in ASCII)
          i = i + 2; // Move to the next number (+1, previous number, +1 for the space)
        }
        else {
          scores[j] = move_score[i + offset] - 48; // - 48 as 0-9 digits start from 48 in ASCII
          i = i + 2; // Move to the next number (+1, previous number, +1 for the space)
        }
      }
    }  
  }
  // Close the file
  LUT_file.close(); 
  return scores; // Return the scores vector
}

// Read Look Up Table and Compare bitmaps - return equivalent move 
std::string readLUT_compareBitmap(std::string bitMaptoCompare, int LUT) {
  
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
    }  
  }

  LUT_file.close(); // Close the file

  return moveOutput; // Return the move that matches
}
