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
std::vector<int> readLUT_compareBitmap(std::string bitMaptoCompare, int LUT); // Compares all currently found Bitmaps - returns known vector if 


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


 // Create and open a text file
 //std::ofstream MyFile("/home/project/projects/Solver_Pascal/LUT/LUT_5.txt"); // Destructive!


 readLUT_compareBitmap("100200020000001000000000000000000000000000",4);
 /*


 Solver solver; // Create an object


 bool weak = false; // select weak or strong solver


 std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings
 std::vector<int> colScores; // Vector containing the column scores
 std::vector<int> rankedCols;


 // Find the score for each move in each column
 for (int i_1 = 0; i_1 < 7; i_1++) {
   for (int i_2 = 0; i_2 < 7; i_2++) {
     for (int i_3 = 0; i_3 < 7; i_3++) {
       for (int i_4 = 0; i_4 < 7; i_4++) {


         //solver.reset(); // Reset the solver
         std::string move = colNumbers[i_1] + colNumbers[i_2] + colNumbers[i_3] + colNumbers[i_4]; // Create the move


         // Convert move to bitmap 
         int col_height[7] = {0,0,0,0,0,0,0};
         int bitmap[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


         bitmap[i_1 + (7 * col_height[i_1])] = 1; // Add 2 to 
         col_height[i_1] = col_height[i_1] + 1; // Increment col height
         bitmap[i_2 + (7 * col_height[i_2])] = 2; // Add 1 to 
         col_height[i_2] = col_height[i_2] + 1; // Increment col height
         bitmap[i_3 + (7 * col_height[i_3])] = 1; // Add 2 to 
         col_height[i_3] = col_height[i_3] + 1; // Increment col height
         bitmap[i_4 + (7 * col_height[i_4])] = 2; // Add 1 to 
         col_height[i_4] = col_height[i_4] + 1; // Increment col height
         
         std::string bitmap_srt;
         for (int j = 0; j < 42; j++) {
           bitmap_srt = bitmap_srt + std::to_string(bitmap[j]);
         }


         std::vector<int> scores;
         scores = readLUT_compareBitmap(bitmap_srt, 4);


       } 
     }
   }
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


// Read Look Up Table and Compare bitmaps - return vector of moves if matching score found 
std::vector<int> readLUT_compareBitmap(std::string bitMaptoCompare, int LUT) {
 
 std::string LUT_str = std::to_string(LUT); // Convert LUT variable to a string
 std::string filename = "/home/project/projects/Solver_Pascal/LUT/LUT_" + LUT_str + ".txt"; // Create file name to index correct LUT
 std::ifstream LUT_file(filename); // Read from the text file


 std::string readLine;// Create a string for output of the file
 std::string bitmapInFile;// Create a string for output of the file
 std::string moveOutput;
 std::vector<int> scores = {0,0,0,0,0,0,0}; // scores vector
 
 // Read function line by line
 while (getline (LUT_file, readLine)) {
   bitmapInFile = readLine.substr((LUT+1),42); // Get the bitmap on a line


   if (bitmapInFile == bitMaptoCompare) { // Compare the bitmaps to check if any match
     moveOutput = readLine.substr((0),LUT); // Get the bitmap on a line
     std::cout << moveOutput << std::endl;
   }  
 }


 // Close the file
 LUT_file.close(); 
 return scores; // Return the scores vector
}
/* Read file and Output the result
 std::vector<int> scores;
 scores = readLUT(""); // NOT PROTECTED AGAINST FALSE INPUTS


 for (int i = 0; i < 7; i++) {
   std::cout << scores[i] << " ";
 }
 std::cout << std::endl;
*/
/* Write to file
 MyFile << move << " ";
 for (int i = 0; i < 42; i++) {
   MyFile << bitmap[i];
 }
 MyFile << std::endl;
 MyFile.close(); // Close the file  
*/






/* Solve fuction
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


       
       for (int i = 0; i < 7; i++) {
         std::cout << colScores[i] << " ";
       }
*/
























//Game loop


/*
* while moves length != 42
* update moves 
* score next move
* sort next moves scores --> indicies of sort (+1 = col number)
* instruct user which one to play in 
* user inputs which column they played in 
*/




/* Save of the Main function
int main(int argc, char** argv) {


 Solver solver; // Create an object


 bool weak = false; // select weak or strong solver


 std::string moves = "445333"; // String of moves
 std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings
 std::vector<int> colScores; // Vector containing the column scores
 std::vector<int> sortedColScores; // Vector containing the sorted scores
 std::vector<int> rankedCols;


 solver.reset();


 // Find the score for each move in each column
 for (int i = 0; i < 7; i++) {
   moves = moves + colNumbers[i]; // Add column number to test


   Position P;


   if(P.play(moves) != moves.size()) { // Error Check
     std::cerr << "Invalid move " << (P.nbMoves()+1) << " \"" << moves << "\"" << std::endl;
   }
   else {
     
     int score = solver.solve(P, weak); // Find the score of the new potential move
     score = - score; // Complement the score !! OUTPUT OF SOLVER IS INVERTED FOR SOME REASON
     colScores.insert(colScores.begin() + i, score); // Add the scores to score vector
     sortedColScores.insert(sortedColScores.begin() + i, score); // Add the scores to sorted score vector


     std::cout << "Column " << i + 1 << " score = " << score << std::endl; // Output column number and score
   }
   moves.pop_back(); // remove the last column number ready for the next column number to be tested
 }


 // Sort scores
 sort(sortedColScores.begin(), sortedColScores.end(), compare);


 // Match scores to column numbers
 for (int i = 0; i < 7; i++) {
   // compare each element of sorted colscores to unsorted


   bool match = false;
   int j = 0; // Index for search 


   while (match == false) {
     if (sortedColScores[i] == colScores[j]) { 
       rankedCols.insert(rankedCols.begin() + i, j + 1); // add column number in order
       colScores[j] = sortedColScores[0] + 1; // Set foud element to be the highest score plus one - ensures 7 unique columns are found 
       match = true; // Set bool as true as a match has been found
     }
     else {
       j++;
     }
   }
 }


 // Output scores
 std::cout << "Ordered Scores: ";
 for (int i = 0; i < 7; i++) {
   std::cout << sortedColScores[i] << " ";
 }
 std::cout << std::endl;


 std::cout << "Ordered Columns: ";
 for (int i = 0; i < 7; i++) {
   std::cout << rankedCols[i] << " ";
 }
 std::cout << std::endl;
}
*/


/* Main function Pre me messing with it
#include "solver.hpp"
#include <iostream>
#include <sys/time.h>


using namespace GameSolver::Connect4;


unsigned long long getTimeMicrosec() {
 timeval NOW;
 gettimeofday(&NOW, NULL);
 return NOW.tv_sec*1000000LL + NOW.tv_usec;
}




int main(int argc, char** argv) {


 Solver solver;


 bool weak = false;
 if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'w') weak = true;


 std::string line;


 for(int l = 1; std::getline(std::cin, line); l++) {
   Position P;
   if(P.play(line) != line.size())
   {
     std::cerr << "Line " << l << ": Invalid move " << (P.nbMoves()+1) << " \"" << line << "\"" << std::endl;
   }
   else
   {
     solver.reset();
     unsigned long long start_time = getTimeMicrosec();
     int score = solver.solve(P, weak);
     unsigned long long end_time = getTimeMicrosec();
     std::cout << line << " " << score << " " << solver.getNodeCount() << " " << (end_time - start_time);
   }
   std::cout << std::endl;
 }
}
*/
