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

Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness);

// Get image: rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png

int main(int argc, char** argv) 
{
    // Colour Detection Variables
    Vec3b pixelColour;
    int blue;
    int green;
    int red;

    int gameState[42]; // Array to store the game state

    // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground.jpg");
    
    // Check if image is found
    if (!img.data) {
        cout << "No image data\n" << endl;
        return -1;
    }

    // -------------------------------------------- Image Processing--------------------------------------------
    
    // Resize Image
    float SF = 0.5; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor

    /*
    int w = img.size().width; // W: 2304
    int h = img.size().height; // H: 1296
    //img = img(Range(0.05*h,0.85*h), Range(0.15*w,0.75*w)); // Crop image
    */

    Mat gray, gray_blur, edge; // Create Mat variables for storing the images
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    GaussianBlur(gray, gray_blur, Size(9, 9), 2, 2 ); // Blur the image
    Canny(gray_blur,edge,125,150,3,true); // Edge detection
    imshow("Edge detection",edge); waitKey(0);

    // detect the contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // draw contours on the original image
    Mat image_copy = img.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    imshow("Contours",image_copy); waitKey(0);

    /*
    
    // Circle Detection
    vector<Vec3f> circles;
    HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,50,59);
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
        // Get Coordinates and radius from Circles vector
        float x = cvRound(circles[i][0]);
        float y = cvRound(circles[i][1]);
        int radius = cvRound(circles[i][2]);
        
        Point center(x,y); // Calculate centre point 
        //cout << "(" << x << "," << y << ")\n"; // Display Coordiatnes

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
        imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/realGame_whiteBackground.jpg",img); 
        cout << "Image Saved" << endl;
    }

    /* */

    return 0;
}

bool sortYCoord(const Vec3f &a, const Vec3f &b) {
    return a[1] < b[1];
}
bool sortXCoord(const Vec3f &a, const Vec3f &b) {
    return a[0] < b[0];
}

Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness) {
    // Alpha value for Contrast [1.0-3.0]
    // Beta value for Brightness [0-100]

    Mat outputImg = Mat::zeros(inputImg.size(), inputImg.type()); // Create new Mat variable to store new image

    for( int y = 0; y < inputImg.rows; y++ ) {
        for( int x = 0; x < inputImg.cols; x++ ) {
            for( int c = 0; c < inputImg.channels(); c++ ) {
                outputImg.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(contrast*inputImg.at<Vec3b>(y,x)[c] + brightness);
            }
        }
    }
    return outputImg;
}

/*
- work out distance to the board 
- Scale image accordingly
- Find board and crop

- Colour calibration algorithm 
- Change colour check to HSV
- Probe multiple points 

    // ----------------------------- Masking the Board ---------------------------------------------
    Vec3b hsvPixel(150, 100, 100); // Create a HSV pixel as a vector

    // Threshold Values for each quantity
    int hue_thresh = 20;
    int sat_thresh = 65;
    int val_thresh = 100;

    // Calculate min and max values
    Scalar minHSV = Scalar(max(0,hsvPixel.val[0] - hue_thresh), max(0,hsvPixel.val[1] - sat_thresh), max(0,hsvPixel.val[2] - val_thresh));
    Scalar maxHSV = Scalar(min(360,hsvPixel.val[0] + hue_thresh), min(255,hsvPixel.val[1] + sat_thresh), min(255,hsvPixel.val[2] + val_thresh));

    Mat mask, maskedBoard;
    inRange(img, minHSV, maxHSV, mask);
    bitwise_and(img, img, maskedBoard, mask);
    imshow("Maskedboard",maskedBoard); waitKey(0);

    Mat maskedBoard_gray;
    cvtColor(maskedBoard, maskedBoard_gray, COLOR_BGR2GRAY);
    imshow("Maskedboard Gray",maskedBoard_gray); waitKey(0);
*/
