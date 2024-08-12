#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <algorithm> // For sort

#include "chrono" // For time tracking

#include <string>

//using namespace std;
using namespace cv;

int gameState[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,2,0,2,2,1,1};
std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings

std::vector<int> scores = {-4,-18,2,-1,-2,18,-4};
std::vector<int> colRank = {6,3,4,5,1,7,2};

int GUI_game(int pascal_col); // pascal_col [0-6]
int GUI_PascalMove();

int main() {

  GUI_game(3);

  
  int i = 3;
  int val;

  while ((i < 7 && i > -1)) {
    val = GUI_game(i);
    if (val == 97) {i--;}
    else if (val == 100) {i++;}

    for (int j = 0; j < 7; j++) {
      scores[j] = ((rand() % 42) - 21);
    }
    
  }
  /**/
  
  destroyAllWindows(); 
}

int GUI_game(int pascal_col) {
  // Varibles for screen size and object sizes
  int screen_width = 800;
  int screen_height = 480;
  int circle_radius = 27;
  int board_space = 8; // Space between circles
  // Remaing Elements are based on these sizes

  // slot = 27mm / space = 8mm

  // Board size
  int board_width = (7 * (circle_radius + circle_radius + board_space)) + board_space; // (7 * (25+25+10)) + 10
  int board_height = (6 * (circle_radius + circle_radius + board_space)) + board_space; // (6 * (25+25+10)) + 10

  // Board coordinates in screen: (X1,Y1) = Top-Left Corner, (X2,Y2) = Bottom-Right Corner
  int board_Y_offset = 36; // offset to lift board from bottom edge
  int X1 = (screen_width - board_width) / 2; // TL: X1 = (screen_width - board_width) / 2
  int Y1 = screen_height - board_height - board_Y_offset; // TL: Y1 = screen_height - board_height - offset
  int X2 = board_width + ((screen_width - board_width) / 2); // BR: X1 = X1 + board_width = board_width + ((screen_width - board_width) / 2)
  int Y2 = screen_height - board_Y_offset; // BR: Y2 = screen_height - offset
  Point TL_board = {X1,Y1}; // Top Left corner of the board
  Point BR_board = {X2,Y2}; // Bottom right corner of the board

  // Board Legs
  // Left leg: From LHS,4/5 down --> 1/2 radius from LHS, screen height
  // Right leg: From RHS,4/5 down --> 1/2 radius from RHS, screen height
  int leg_startHeight = 0.75 * board_height;
  int leg_width = 0.7 * circle_radius;
  Point leg_L_top = {X1,Y1 + leg_startHeight};
  Point leg_L_bot = {X1 - leg_width, screen_height};
  Point leg_R_top = {X2,Y1 + leg_startHeight};
  Point leg_R_bot = {X2 + leg_width, screen_height};

  // Colours
  const Scalar background_colour = {255,245,229};
  const Scalar background_circles_colour = {255,241,214};

  const Scalar board_colour = {255,20,20}; // Blue
  const Scalar board_outline_colour = {220,0,0}; // Dark Blue
  const Scalar red_counter_colour = {0,0,255}; // Red
  const Scalar yel_conuter_colour = {0,255,255}; // Yellow
  const Scalar empty_slot_colour = {255,255,255}; // White

  const Scalar Pascal_move_indicator_colour = {81,213,66}; // Green 
  const Scalar Pascal_move_indicator_outline_colour = {70,185,58}; // Darker Green 

  // -------------------------------------------Create the image-------------------------------------------
  Mat display(screen_height, screen_width, CV_8UC3, background_colour); // create empty screen - white background

  // Add background details
  std::vector<Point> backbround_circle_centres = {{40,10},{400,250},{670,200},{200,400},{40,260},{700,400},{460,410},{200,260},{700,40},{220,70},{500,100}};
  std::vector<int> backbround_circle_radius = {50,80,40,60,50,100,40,40,50,65,50};
  for (int i = 0; i < 11; i++) {
    cv::circle(display,backbround_circle_centres[i],backbround_circle_radius[i],background_circles_colour,FILLED,LINE_8,0); // Draw background circles
  }

  // -------------------------------------------Draw the Board-------------------------------------------
  cv::rectangle(display,TL_board,BR_board,board_colour,FILLED,LINE_8,0); // Draw the board
  cv::rectangle(display,TL_board,BR_board,board_outline_colour,2,LINE_8,0); // Draw the board outline

  // Draw legs
  cv::rectangle(display,leg_L_top,leg_L_bot,board_colour,FILLED,LINE_8,0); // Draw the left board leg
  cv::rectangle(display,leg_R_top,leg_R_bot,board_colour,FILLED,LINE_8,0); // Draw the right board leg

  cv::rectangle(display,leg_L_top,leg_L_bot,board_outline_colour,2,LINE_8,0); // Draw the left board leg
  cv::rectangle(display,leg_R_top,leg_R_bot,board_outline_colour,2,LINE_8,0); // Draw the right board leg

  // -------------------------------------------Draw the Counters-------------------------------------------
  Point circle_centre;
  int circle_X;
  int circle_Y;
  
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      // Calculate centre points for each counter slot
      // Circle centre X coordinate = X1 + space + radius + (j * (radius+radius+space))
      // Circle centre Y coordinate = Y1 + space + radius + (i * (radius+radius+space))
      circle_X = X1 + board_space + circle_radius + (j * (circle_radius+circle_radius+board_space));
      circle_Y = Y1 + board_space + circle_radius + (i * (circle_radius+circle_radius+board_space));
      circle_centre = {circle_X,circle_Y};

      // Draw the counters/slots based on the corresponding element in the gameState array
      if (gameState[(i * 7) + j] == 0) {
        cv::circle(display,circle_centre,circle_radius,empty_slot_colour,FILLED,LINE_8,0); // Draw as white if empty
      }
      else if (gameState[(i * 7) + j] == 1) {
        cv::circle(display,circle_centre,circle_radius,red_counter_colour,FILLED,LINE_8,0); // Draw as red if element == 1
      }
      else if (gameState[(i * 7) + j] == 2) {
        cv::circle(display,circle_centre,circle_radius,yel_conuter_colour,FILLED,LINE_8,0); // Draw as yellow if element = 2
      }

      cv::circle(display,circle_centre,circle_radius,board_outline_colour,2,LINE_8,0); // Draw outline around circles
    }
  }

  // -------------------------------------------Draw the Indicator Triangle-------------------------------------------
  // Draw triangular pointer to indicate pascal's move
  // Triangle defined by the lowest point (tri_X and tri_Y) and the base and height
  int tri_base = circle_radius; // Triangle base is same size as circle radius
  int tri_height = circle_radius * 1.7; // Triangle height is double the circle radius
  int tri_Y = Y1 - (circle_radius / 3); // offset shifts the triangle N pixels above the board (N = circle radius)
  int tri_X = X1 + board_space + circle_radius; // Initial offset for x-axis (over 1st column)
  
  // Set the indicator arrow to be above the desired column
  tri_X = tri_X + (pascal_col * (circle_radius + board_space + circle_radius));

  Point p1 = {tri_X,tri_Y}; // Bottom point of the triangle
  Point p2 = {tri_X - (tri_base / 2),tri_Y - tri_height};
  Point p3 = {tri_X + (tri_base / 2),tri_Y - tri_height};

  // Place the triangle vertices into a vector
  std::vector<cv::Point> trianglePoints;
  trianglePoints.push_back(p1);
  trianglePoints.push_back(p2);
  trianglePoints.push_back(p3);

  // Create a vector of vectors of points for the fillPoly function
  std::vector<std::vector<cv::Point>> fillContAll;
  fillContAll.push_back(trianglePoints);

  // Draw the filled triangle
  cv::fillPoly(display, fillContAll, Pascal_move_indicator_colour);

  // Draw outline
  cv::line(display,p1,p2,Pascal_move_indicator_outline_colour,2,LINE_8,0);
  cv::line(display,p2,p3,Pascal_move_indicator_outline_colour,2,LINE_8,0);
  cv::line(display,p3,p1,Pascal_move_indicator_outline_colour,2,LINE_8,0);
  
  // -------------------------------------------Display Text-------------------------------------------
  // Display Column number
  std::vector <std::string> PascalPlay_text = {"  Pascal", "wishes to"," play in",("Column " + colNumbers[pascal_col])};
  Point text_pos = {X1-180,Y1}; // Bottom-left corner of text - set to be two triangle heights above the board
  for (int i = 0; i < 4; i++) {
    putText(display,PascalPlay_text[i],{X1 - 168,Y1 + 125  + (i*30)},FONT_HERSHEY_DUPLEX,1,{0,0,0},1,LINE_AA,false);
  }

  // -------------------------------------------Display Medals to Indicate best moves-------------------------------------------
  Point rank_medal_centre;
  int rank_medal_X = X1 + board_space + circle_radius; // Set initial position
  int rank_medal_Y = Y2 + (board_Y_offset / 2); // Under the board
  int rank_medal_radius = (board_Y_offset - 5) / 2;

  // Medal Colours
  const Scalar gold_rank_colour = {0,215,255};
  const Scalar silver_rank_colour = {192,192,192};
  const Scalar bronze_rank_colour = {83,120,140};

  // Draw medals - position based on ranking 
  rank_medal_centre = {rank_medal_X + ((colRank[0] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
  cv::circle(display,rank_medal_centre,rank_medal_radius,gold_rank_colour,FILLED,LINE_8,0); 

  rank_medal_centre = {rank_medal_X + ((colRank[1] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
  cv::circle(display,rank_medal_centre,rank_medal_radius,silver_rank_colour,FILLED,LINE_8,0); 

  rank_medal_centre = {rank_medal_X + ((colRank[2] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
  cv::circle(display,rank_medal_centre,rank_medal_radius,bronze_rank_colour,FILLED,LINE_8,0); 

  // -------------------------------------------Display Scores-------------------------------------------
  // Need to vary the X offset based on the size of the text - use getTextSize to find width an convert to offset
  int fontFace = FONT_HERSHEY_DUPLEX;
  double fontScale = 0.5;
  int thickness = 1;
  const Scalar textColour = {0,0,0};
  std::string score_str;

  // Varible for score position and the initial offsets
  Point score_pos;
  int score_X = X1 + board_space; // Varies depending on text size
  int score_Y = Y2 + (board_Y_offset / 2) + 5; // Constant!

  /* Char Size vs Text Width Vs Ofset
  // -VE Two Digit | 34 | 8
  // -VE One Digit | 24 | 15
  // +VE One Digit | 11 | 23
  // +VE Two Digit | 21 | 17
  //      "W"      | 14 | 21
  */
  Size textSize;
  int text_X_offset;

  for (int i = 0; i < 7; i++) { // For each score
    // Set output text
    if (scores[i] == 77) {
      score_str = "W";
    }
    else if (scores[i] == -77) {
      score_str = " ";
    }
    else if (scores[i] != -77) {
      score_str = std::to_string(scores[i]);
    }

    // Vary the offset based on the text size
    textSize = getTextSize(score_str,fontFace,fontScale,thickness,0);
    switch (textSize.width) {
      case 34: // -VE Two Digit
        text_X_offset = 8;
        break;
      case 24: // -VE One Digit
        text_X_offset = 15;
        break;
      case 11: // +VE One Digit
        text_X_offset = 23;
        break;
      case 21: // +VE Two Digit
        text_X_offset = 17;
        break;
      case 14: // "W"
        text_X_offset = 21;
        break;
      defalut:
        text_X_offset = 0;
    }

    // Set Position and output the text
    score_pos = {score_X + text_X_offset + (i * (board_space + circle_radius + circle_radius)),score_Y}; // Set the text position
    putText(display,score_str,score_pos,fontFace,fontScale,textColour,thickness,LINE_AA,false); // Output the text
  }

 //-------------------------------------------Display the Image-------------------------------------------
  cv::imshow("Display",display); 
  int val = cv::waitKey(0); // display the image
  return val;
}
