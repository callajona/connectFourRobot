#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
  Mat image = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png");

  // Check if image is found
  if (!image.data)
  {
      cout << "No image data\n" << endl;
      return -1;
  }
  
  // Find Size of the Image
  Size image_size = image.size();

  int width = image_size.width; //226
  int height = image_size.height; //194

  cout << "Width: " << width << "\nHeight: " << height << endl;

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
  char colours[6][7];

  // Search each row and column
  for (int i = 0; i < 6; i++) 
  {
      y = 16 + (i * 32); // Set / Increment Y

      for (int j = 0; j < 7; j++)
      {
          x = 16 + (j * 32); // Set / Increment X

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
          else if (blue > 125 && red < 125 && green < 125) {
              colours[i][j] = 'B';
          }
      }
  }

  // Display Grid of colours
  for (int i = 0; i < 6; i++) 
  {
      for (int j = 0; j < 7; j++)
      {
          cout << " " << colours[i][j];
      }
      cout << "\n"; 
  }

  return 0;
}
