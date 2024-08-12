#include "GUI.hpp"
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>

using namespace cv;

// -------------------------------------------Define varibles for Screen-------------------------------------------
int screen_width = 800;
int screen_height = 480;

// Size of the board elements - Measured: slot = 27mm | space = 8mm
int circle_radius = 27;
int board_space = 8; // Space between circles

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

// Points and radius for background details
std::vector<Point> backbround_circle_centres = {{40,10},{400,250},{670,200},{200,400},{40,260},{700,400},{460,410},{200,260},{700,40},{220,70},{500,100}};
std::vector<int> backbround_circle_radius = {50,80,40,60,50,100,40,40,50,65,50};

// Circle Variables
Point circle_centre;
int circle_X;
int circle_Y;

// Medal Variables
Point rank_medal_centre;
int rank_medal_X = X1 + board_space + circle_radius; // Set initial position
int rank_medal_Y = Y2 + (board_Y_offset / 2); // Under the board
int rank_medal_radius = (board_Y_offset - 5) / 2;

// -------------------------------------------Colours-------------------------------------------
const Scalar background_colour = {255,245,229};
const Scalar background_circles_colour = {255,241,214};

const Scalar board_colour = {255,20,20}; // Blue
const Scalar board_outline_colour = {220,0,0}; // Dark Blue
const Scalar red_counter_colour = {0,0,255}; // Red
const Scalar yel_conuter_colour = {0,255,255}; // Yellow
const Scalar empty_slot_colour = {255,255,255}; // White

const Scalar Pascal_move_indicator_colour = {81,213,66}; // Green 
const Scalar Pascal_move_indicator_outline_colour = {70,185,58}; // Darker Green 

// Medal Rank Colours
const Scalar gold_rank_colour = {0,215,255};
const Scalar silver_rank_colour = {192,192,192};
const Scalar bronze_rank_colour = {83,120,140};

// Colours for difficulty select
const Scalar easy_colour = {222,254,200};
const Scalar medium_colour = {205,236,254};
const Scalar hard_colour = {205,205,254};
const Scalar extreme_colour = {255,214,239};

const Scalar easy_outline_colour = {198,227,179};
const Scalar medium_outline_colour = {192,223,240};
const Scalar hard_outline_colour = {192,192,237};
const Scalar extreme_outline_colour = {219,184,206};
std::vector<Scalar> diff_sel_colours = {easy_colour,medium_colour,hard_colour,extreme_colour};
std::vector<Scalar> diff_sel_outline_colours = {easy_outline_colour,medium_outline_colour,hard_outline_colour,extreme_outline_colour};

// -------------------------------------------Text Parameters-------------------------------------------
int fontFace = FONT_HERSHEY_DUPLEX;
double text_fontScale = 1; // Font size for text displayed to the screen
double score_fontScale = 0.5; // Font size for scores
int thickness = 1;
const Scalar textColour = {0,0,0};

// Necessary Variables
int gameState[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,2,1,0,2,0,2,2,1,1};
std::string colNumbers[7] = {"1","2","3","4","5","6","7"}; // Column numbers as strings

int GUI_game(int pascal_col, bool Pascal_move, bool display_scores, std::vector<int> scores, std::vector<int> colRank) {
  // -------------------------------------------Create the image-------------------------------------------
  Mat display(screen_height, screen_width, CV_8UC3, background_colour); // create empty screen - white background

  // Add background details
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

  if (Pascal_move == true) {
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
    Size textSize;
    int text_X_offset;

    // Display Column number
    std::vector <std::string> PascalPlay_text = {"Pascal", "wishes to","play in",("Column " + colNumbers[pascal_col])};

    int text_Y = Y1 + 125; // Fixed
    
    for (int i = 0; i < 4; i++) {
      textSize = getTextSize(PascalPlay_text[i],fontFace,text_fontScale,thickness,0); // Find the size of the text
      int text_X = (X1 - textSize.width) / 2; // Calculate the X offset
      int text_height_offset = textSize.height * 1.5; // Calculate Y offset for new line
      putText(display,PascalPlay_text[i],{text_X, text_Y + (i * text_height_offset)},fontFace,text_fontScale,textColour,thickness,LINE_AA,false);
    }
  }
  else if (Pascal_move == false){
    // -------------------------------------------Display Text-------------------------------------------
    // Need to vary the X offset based on the size of the text - use getTextSize to find width an convert to offset
    Size textSize;
    int text_X_offset;

    // Display Column number
    std::vector <std::string> userPlay_text = {"Your", "turn!"};

    int text_Y = Y1 + 125; // Initial Y-offset. Varies with 

    for (int i = 0; i < 2; i++) {
      textSize = getTextSize(userPlay_text[i],fontFace,text_fontScale,thickness,0); // Find the size of the text
      int text_X = (X1 - textSize.width) / 2; // Calculate the X offset
      int text_height_offset = textSize.height * 1.5; // Calculate Y offset for new line
      putText(display,userPlay_text[i],{text_X, text_Y + (i * text_height_offset)},fontFace,text_fontScale,textColour,thickness,LINE_AA,false);
    }
  }

  if (display_scores == true) {
    // -------------------------------------------Display Medals to Indicate best moves-------------------------------------------
    // Draw medals - position based on ranking 
    rank_medal_centre = {rank_medal_X + ((colRank[0] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
    cv::circle(display,rank_medal_centre,rank_medal_radius,gold_rank_colour,FILLED,LINE_8,0); 

    rank_medal_centre = {rank_medal_X + ((colRank[1] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
    cv::circle(display,rank_medal_centre,rank_medal_radius,silver_rank_colour,FILLED,LINE_8,0); 

    rank_medal_centre = {rank_medal_X + ((colRank[2] - 1)* (circle_radius + board_space + circle_radius)),rank_medal_Y};
    cv::circle(display,rank_medal_centre,rank_medal_radius,bronze_rank_colour,FILLED,LINE_8,0); 

    // -------------------------------------------Display Scores-------------------------------------------
    // Need to vary the X offset based on the size of the text - use getTextSize to find width an convert to offset
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
      textSize = getTextSize(score_str,fontFace,score_fontScale,thickness,0);
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
      putText(display,score_str,score_pos,fontFace,score_fontScale,textColour,thickness,LINE_AA,false); // Output the text
    }
  }

 //-------------------------------------------Display the Image-------------------------------------------
  cv::imshow("Display",display); 
  int val = cv::waitKey(0); // display the image
  return val;
}

int GUI_difficulty_sel(int diff_sel) {
  // -------------------------------------------Create the image-------------------------------------------
  Mat display(screen_height, screen_width, CV_8UC3, background_colour); // create empty screen - white background

  // Add background details
  for (int i = 0; i < 11; i++) {
    cv::circle(display,backbround_circle_centres[i],backbround_circle_radius[i],background_circles_colour,FILLED,LINE_8,0); // Draw background circles
  }

  // -------------------------------------------Display Text-------------------------------------------
  std::string selDiff_text = {"Select Difficulty: "};
  std::vector<std::string> diffLevels_text = {"  Easy >","< Medium >","< Hard >","< Extreme  "};
  Size selDiff_textSize = getTextSize(selDiff_text,fontFace,text_fontScale,thickness,0); // Find the size of the text

  int selDiff_text_X = (screen_width - selDiff_textSize.width) / 2; // Calculate the X offset
  int selDiff_text_Y = screen_height * 0.25; // Fixed
  putText(display,selDiff_text,{selDiff_text_X, selDiff_text_Y},fontFace,text_fontScale,textColour,thickness,LINE_AA,false);

  Size diffLevels_textSize = getTextSize(diffLevels_text[diff_sel],fontFace,text_fontScale,thickness,0); // Find the size of the text
  int diffLevels_text_X = (screen_width - diffLevels_textSize.width) / 2; // Calculate the X offset
  int diffLevels_text_Y = screen_height * 0.5; // Fixed Y offset

  // Bounding rectangle points
  // BR_X1 = diffLevels_text_X
  int BR_X2 = diffLevels_text_X + diffLevels_textSize.width;
  int BR_Y1 = (screen_height - diffLevels_textSize.height) / 2;
  int BR_Y2 = BR_Y1 + diffLevels_textSize.height;
  Point TL_BR = {diffLevels_text_X,BR_Y1}; // Top-Left Bounding rectangle
  Point BR_BR = {BR_X2,BR_Y2}; // Bottom-Right Bounding Rectangle
  cv::rectangle(display,TL_BR,BR_BR,diff_sel_colours[diff_sel],FILLED,LINE_AA,0);
  cv::rectangle(display,TL_BR,BR_BR,diff_sel_outline_colours[diff_sel],2,LINE_AA,0);

  putText(display,diffLevels_text[diff_sel],{diffLevels_text_X, diffLevels_text_Y},fontFace,text_fontScale,textColour,thickness,LINE_AA,false);

 //-------------------------------------------Display the Image-------------------------------------------
  cv::imshow("Display",display); 
  int val = cv::waitKey(0); // display the image
  return val;
}