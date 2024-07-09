#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <algorithm> // For sort

using namespace std;
using namespace cv;

// --- Prorotypes ---
bool sortYCoord(const Vec3f &a, const Vec3f &b);
bool sortXCoord(const Vec3f &a, const Vec3f &b);

// Get image: rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourNoBackground.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png

int main(int argc, char** argv) 
{
    int gameState[42]; // Array to store the game state

    // Colour Detection Variables
    Vec3b pixelColour;
    int blue;
    int green;
    int red;

    // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourNoBackground.jpg");
    
    
    // Check if image is found
    if (!img.data) {
        cout << "No image data\n" << endl;
        return -1;
    }

    // -------------------------------------------- Image Processing--------------------------------------------
    
    // Resize Image
    float SF = 0.5; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor

    // Convert image to greyscale and blur the grayscale image
    Mat gray, gray_blur; // Create gray to hold grayscale image and gray_blur to hold the blurred gray scale image
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    GaussianBlur(gray, gray_blur, Size(9, 9), 2, 2 ); // smooth it, otherwise a lot of false circles may be detected

    // Masking the Board
    Vec3b hsvPixel(170, 50, 100); // Create a HSV pixel as a vector

    // Threshold Values for each quantity
    int hue_thresh = 30;
    int sat_thresh = 50;
    int val_thresh = 100;

    // Calculate min and max values
    Scalar minHSV = Scalar(max(0,hsvPixel.val[0] - hue_thresh), max(0,hsvPixel.val[1] - sat_thresh), max(0,hsvPixel.val[2] - val_thresh));
    Scalar maxHSV = Scalar(min(360,hsvPixel.val[0] + hue_thresh), min(255,hsvPixel.val[1] + sat_thresh), min(255,hsvPixel.val[2] + val_thresh));

    Mat mask, maskedBoard;
    inRange(img, minHSV, maxHSV, mask);
    bitwise_and(img, img, maskedBoard, mask);

    /*
    // Edge detection
    Mat edge;
    Canny(maskedBoard,edge,125,150,3,true);
    */
     
    // Circle Detection
    vector<Vec3f> circles;
    HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,52,57);
    //HoughCirles(in,out,TYPE,resulution SF, no. circles per width, param1, param2, min radius, max radius)
    // variables that worked for the gemerated image: HoughCircles(gray, circles, HOUGH_GRADIENT,2, gray.rows/9, 300, 110);
    // Variables that worked for real image: HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,52,57);
    cout << "Number of Circles: " << circles.size() << endl; // Output number of circles

    // Sort Circles Vector in grid order
    sort(circles.begin(), circles.end(), sortYCoord); // Sort all vectors based on y-coordinates (rows still scrambled: e.g. 3 2 1; 4 6 5; 9 7 8)
    for (int i = 0; i < 6; i++) {
        sort(circles.begin() + (i*7), circles.begin() +7 + (i*7),sortXCoord); // Sort each row
    }

    for(size_t i = 0; i < circles.size(); i++) {
        // Get Coordinates from Circles vector
        float x = cvRound(circles[i][0]);
        float y = cvRound(circles[i][1]);
        //cout << "(" << x << "," << y << ")\n"; // Display Coordiatnes
        
        // Calculate centre point and find radius from vector
        Point center(x,y);
        int radius = cvRound(circles[i][2]);

        // Fing BGR values at coordinates and split into indiviual varaiables
        pixelColour = img.at<Vec3b>(y, x); 
        blue = pixelColour[0];
        green = pixelColour[1];
        red = pixelColour[2];
        //cout << i << " - B: " << blue << "  G: " << green << "  R: " << red << endl; // Display colours
        
        // --- Draw circles on image ---
        circle( img, center, radius, Scalar(255,0,0), 3, 8, 0 ); // draw the circle outline

        // Draw centre cirlce - Change colour depending on colour detected
        if (red > 130 && blue < 100 && green < 100) {
            circle( img, center, 10, Scalar(0,0,255), -1, 8, 0 ); // draw the circle center - red when counter is red
            gameState[i] = 1;
        }
        else if (red > 170 && blue < 100 && green > 175) {
            circle( img, center, 10, Scalar(0,255,255), -1, 8, 0 ); // draw the circle center - yellow when counter is yellow
            gameState[i] = 2;
        }
        else {
            circle( img, center, 10, Scalar(0,0,0), -1, 8, 0 ); // draw the circle center - Black when no counter
            gameState[i] = 0;
        }        
    }

    // Display Game State in Terminal
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            cout << gameState[j+(i*7)] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    // Create window to display image
    namedWindow( "Image", 1 );
    imshow( "Image", img );
    int key = waitKey(0);

    // Save image
    if (key == 's') {
        imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/realGame_noBackground.jpg",img); 
        cout << "Image Saved" << endl;
    }

    return 0;
}

bool sortYCoord(const Vec3f &a, const Vec3f &b) {
    return a[1] < b[1];
}

bool sortXCoord(const Vec3f &a, const Vec3f &b) {
    return a[0] < b[0];
}
