#ifndef GUI_HPP
#define GUI_HPP

#include <opencv2/opencv.hpp>

int GUI_game(int pascal_col, bool Pascal_move, bool display_scores, std::vector<int> scores, std::vector<int> colRank);
int GUI_difficulty_sel(int diff_sel);


#endif