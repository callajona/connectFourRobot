#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  // Import image into code
  Mat image = imread("C:/Users/Jonathan/OneDrive - University of Leeds/Documents/Work/openCV_Test_3/basicConnectFourExample/mockUp.png");
  // Change image directory and image name before use

  // Check if image is found
  if (!image.data)
  {
      cout << "No image data\n" << endl;
      return -1;
  }

  // Declair coordinate varibles - init (12,12) = centre of 1 square
  int x;
  int y;

  // BGR Levels Variables
  int blue; // pixelColour[0];
  int green; // pixelColour[1];
  int red;   // pixelColour[2];

  // Variable for the pixel colour
  Vec3b pixelColour;

  // Create array to store colors
  char colours[4][4];

  // Search each row and column
  for (int i = 0; i < 4; i++) 
  {
      y = 12 + (i * 25); // Set / Increment Y

      for (int j = 0; j < 4; j++)
      {
          x = 12 + (j * 25); // Set / Increment X

          pixelColour = image.at<Vec3b>(y, x); // Fing BGR values at coordinates

          // get values for each BGR level 
          blue = pixelColour[0];
          green = pixelColour[1];
          red = pixelColour[2];

          // Convert the BGR values to letter representations
          if (red == 255 && green == 255 && blue == 255) {
              colours[i][j] = 'W';
          }
          else if (blue < 125 && red > 125 && green > 125) {
              colours[i][j] = 'Y';
          }
          else if (blue < 125 && red > 125 && green < 125) {
              colours[i][j] = 'R';
          }
      }
  }

  // Display Grid of colours
  for (int i = 0; i < 4; i++) 
  {
      for (int j = 0; j < 4; j++)
      {
          cout << " " << colours[i][j];
      }
      cout << "\n"; 
  }

  return 0;
}
