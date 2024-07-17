#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp> // For frame capture?
#include <math.h>
#include <algorithm> // For sort
#include "chrono"

using namespace std;
using namespace cv;

// --- Prorotypes ---
bool sortYCoord(const Point &a, const Point &b);
bool sortXCoord(const Point &a, const Point &b);
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2); // comparison function object

Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness);

// Get image: rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground3.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png

int main(int argc, char** argv) 
{
    const auto start{std::chrono::steady_clock::now()}; // Start Clock

    int gameState[42];
    int actual_gameState[42] = {
        0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,
        0,0,2,1,2,2,1,
        1,0,1,2,1,1,2,
        2,1,2,1,2,2,1,
        1,2,2,1,1,2,1
        };

    // -------------------------------------------- Import Image --------------------------------------------
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground3.jpg");
    
    // Check if image is found
    if (!img.data) {
        cout << "No image data\n" << endl;
        return -1;
    }

    // -------------------------------------------- Image Processing--------------------------------------------
    
    // Resize Image
    float SF = 0.5; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor
    //imshow("Image",img); waitKey(0);

    // ----------------------------- Masking the Board ---------------------------------------------
    // HSV = Hue, Saturation, Value
    // Hue - Colour [0-180] - Usally 360, but halfed to be stored in a byte
    // Saturation [0-255] - 0 = White, 255 = Full colour
    // Value [0-255] - 0 = Black, 255 = Full Colour

    Mat HSV_img;
    cvtColor(img,HSV_img,COLOR_BGR2HSV); // Convert Image to HSV space 
    
    // Set upper and lower bounds to mask
    Scalar minHSV = Scalar(90,85,60);
    Scalar maxHSV = Scalar(135,255,255);

    // Mask out the board
    Mat mask, maskedBoard;
    inRange(HSV_img, minHSV, maxHSV, mask); // Create board mask by checking if HSV img has pixel in specified range
    bitwise_and(HSV_img, HSV_img, maskedBoard, mask); // Use mask to remove everything else from the image
    //imshow("Mask",mask); waitKey(0); // Display the mask

    Mat BGR_maskedBoard; cvtColor(maskedBoard,BGR_maskedBoard,COLOR_HSV2BGR); // Convert the HSV masked image back into BGR
    //imshow("Origional",BGR_maskedBoard); waitKey(0);

    // -----------------------------Contour-----------------------------
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    Mat contour_img = img.clone(); drawContours(contour_img, contours, -1, Scalar(0,255,0), 2); // Draw contours on the original image
    //imshow("Contours",contour_img); waitKey(0);

    sort(contours.begin(), contours.end(), compareContourAreas); // sort contours

    Rect GameBoard = boundingRect(contours[contours.size()-1]); // Find the rectangle encapsluating the largest contour = boarder

    RotatedRect rotatedRectBoard = minAreaRect(contours[contours.size()-1]); // NOT A ROTATE RECT
    Point2f boardCorners[4];
    rotatedRectBoard.points(boardCorners);

    for (int i = 0; i < 4; i++) {
        cout << boardCorners[i] << endl;
        circle(img, boardCorners[i], 10, Scalar(0,0,0), -1, 8, 0 ); // draw board corners
    }
    
    //Mat croppedGameBoard = img(GameBoard); // Crop image to size of largest contour rectangle - outline of game board
    //Mat boarder_img = img.clone(); drawContours(boarder_img, contours,contours.size()-1, Scalar( 0, 255, 0 ), 2 ); // Draw the largest contour using largest stored index.
    //imshow("Boarder",boarder_img); waitKey(0); // Display the largest contour = boarder of the game 

    /*
    sort(contours.begin(), contours.end(), compareContourAreas); // sort contours

    for (int i = 0; i < contours.size(); i++) {
        cout << contourArea(contours[i]) << endl; // Output area of each contour
    }

    // Remove elements in counters array that aren't the counters
    // - Lower bound removes noise
    // - Upper bound removes the boarder
    for (int i = 0; i < contours.size(); i++) {
        float area = contourArea(contours[i]); // Calculate area of the contour
        if (area < 5000 || area > 20000) {
            contours.erase(contours.begin() + i); // Remove out of range element
            i--; // Removed element so all elements have shifted back, so need to research the previous element; therfore, decrease i
        }
    }

    cout << "Number of Circles: " << contours.size() << endl;

    Mat counterContour_img = img.clone(); drawContours(counterContour_img, contours, -1, Scalar(0,255,0), 2); // Draw counter contours on the original image
    imshow("Counter Contours",counterContour_img); waitKey(0);

    //imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/CA_tooManyContours.jpg",counterContour_img);

    // Check if 42 counters are found
    if (contours.size() != 42) {
        bool stopCheck = false;

        // Elimiante out of range contours
        // Check if in grid formation - elimnate too close together objects

        while (stopCheck == false) {
            // exclude non circular contours
            float perimeter;
            float area; 
            float circularity;

            for (int i = 0; i < contours.size(); i++) {
                perimeter = arcLength(contours[i], true); // Find perimeter of closed shape - true determines closed shape
                area = contourArea(contours[i]);
                circularity = 4*M_PI*(area /(perimeter*perimeter));
                cout << circularity << endl;
                if (circularity < 0.6 || circularity > 1) {
                    contours.erase(contours.begin() + i); // Remove out of range element
                    i--; // Removed element so all elements have shifted back, so need to research the previous element; therfore, decrease i
                }
            }

            if (contours.size() == 42) {
                // End further processing if all 42 circles are identified
                stopCheck = true;
            }
        }        
    }


    /*
    // Find Centre points of contours using the Minimum Enclosing Circle function
    Point2f centre;
    float radius;
    vector<Point> centrePoints; // Vector to store centre points - Point data type to round centre values
    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle(contours[i],centre,radius); // Find the Minimum enclosing circle around the points - gives radius and centre point
        centrePoints.insert(centrePoints.begin() + i, centre); // add centre point to array
    }
    
    // Sort Circles
    sort(centrePoints.begin(), centrePoints.end(), sortYCoord); // Sort all vectors based on y-coordinates (rows still scrambled: e.g. 3 2 1; 4 6 5; 9 7 8)
    for (int i = 0; i < 6; i++) {
        sort(centrePoints.begin() + (i*7), centrePoints.begin() +7 + (i*7),sortXCoord); // Sort each row
    }

    // Detect colours in centre of circles
    Vec3b pixHSV;
    int H, S, V;

    for (int i = 0; i < centrePoints.size(); i++) {
        pixHSV = HSV_img.at<Vec3b>(centrePoints[i]); // Get the HSV values at the centre points of the circle
        
        // Seperate pixel parameters
        H = pixHSV[0];
        S = pixHSV[1];
        V = pixHSV[2];

        // Detect Colours
        if (((H >= 170 && H <= 179) || (H >= 0 && H <= 12)) && (S >= 51 && S <=255) && (V >=77 && V <= 255)) {
            gameState[i] = 1;
        }
        else if ((H >= 18 && H <= 40) && (S >= 51 && S <=255) && (V >=102 && V <= 255)) {
            gameState[i] = 2;
        }
        else {
            gameState[i] = 0;
        }
    }

    // Display Game State in Terminal
    for (int i = 1; i <= 7; i++) {
        cout << i << " "; // Display Column numbers
    }
    cout << "\n";

    // Display individual cells
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            cout << gameState[j+(i*7)] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    // Comapre to actual state of the game and check for errors
    bool errorCheck = false;
    for (int i = 0; i < 42; i++) {
        if (gameState[i] != actual_gameState[i]) {
            cout << "Error in postion: " << i << endl;
            errorCheck = true;
        }
    }

    if (errorCheck == false) {
        cout << "Board Correctly Identified" << endl;
    }

    //imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/CA_ColouredCircles_CorrectlyIdentified.jpg",colouredCircles);

    */

    const auto end{std::chrono::steady_clock::now()}; // end Clock
    const std::chrono::duration<double> elapsed_seconds{end - start};
    cout << "Duration Time: " << elapsed_seconds.count() << endl;

    return 0;
}

bool sortYCoord(const Point &a, const Point &b) {
    return a.y < b.y;
}
bool sortXCoord(const Point &a, const Point &b) {
    return a.x < b.x;
}
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
    double i = fabs( contourArea(Mat(contour1)) );
    double j = fabs( contourArea(Mat(contour2)) );
    return ( i < j );
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
