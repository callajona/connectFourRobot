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

bool compare(int a, int b);

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

  Solver solver; // Create an object

  bool weak = false; // select weak or strong solver

  std::string moves = "44"; // String of moves
  std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings
  std::vector<int> colScores; // Vector containing the column scores
  std::vector<int> sortedColScores; // Vector containing the sorted scores
  std::vector<int> rankedCols;

  while (moves.length() != 42) {
    // Receive input from user as to which column has been played
    std::string colPlayed;
    std::cout << "Enter a Column number to place a counter into: "; 
    std::cin >> colPlayed;

    moves = moves + colPlayed; // add column played to list

  // Find the score for each move in each column
    for (int i = 0; i < 7; i++) {
      moves = moves + colNumbers[i]; // Add column number to test

      Position P;

      if(P.play(moves) != moves.size()) { // Error Check
        std::cerr << "Invalid move " << (P.nbMoves()+1) << " \"" << moves << "\"" << std::endl;
      }
      else {
        solver.reset();
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
}

// Comparison function for sort
bool compare(int a, int b) {return a > b;}


//Game loop

/*
* while moves length != 42
* update moves 
* score next move
* sort next moves scores --> indicies of sort (+1 = col number)
* instruct user which one to play in 
* user inputs which column they played in 
*/













/*
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
