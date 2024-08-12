#include <iostream>
#include "GUI.hpp"

#define PASCAL 1
#define USER 0
#define DISP_SCORES 1
#define NO_SCORES 0

//using namespace std;
using namespace cv;

int main() {
  blank_backgroud();
  int i = GUI_selectDifficulty();

  std::cout << "Level Selected " << i << std::endl;

  std::vector<int> scores = {-4,-18,2,-1,-2,18,-4};
  std::vector<int> colRank = {6,3,4,5,1,7,2};
  GUI_game(2,1,1,scores,colRank);

  int val;

  while ((i < 7 && i > -1)) {
    val = GUI_game(i,PASCAL,DISP_SCORES,scores,colRank);
    if (val == 97) {i--;}
    else if (val == 100) {i++;}

    for (int j = 0; j < 7; j++) {
      scores[j] = ((rand() % 42) - 21);
    }
    
  }
  /**/
  
  destroyAllWindows(); 
}
