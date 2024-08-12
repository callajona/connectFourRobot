#ifndef GUI_HPP
#define GUI_HPP

#include <opencv2/opencv.hpp>

int GUI_game(int pascal_col, bool Pascal_move, bool display_scores, std::vector<int> scores, std::vector<int> colRank); // Display game with different elemets
int GUI_difficultySel_Screen(int diff_sel); // Display screen for difficulty selection
int GUI_selectDifficulty(); // Use FSM to select a difficulty
void blank_backgroud();

#endif
