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
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2); // comparison function object

Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness);

// Get image: rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground2.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png

int main(int argc, char** argv) 
{
    // -------------------------------------------- Import Image --------------------------------------------
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
    //imshow("Image",img); waitKey(0);
    //img = BrightnessAndContrast(img,1,60);

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

    Mat BGR_maskedBoard; 
    cvtColor(maskedBoard,BGR_maskedBoard,COLOR_HSV2BGR); // Convert the HSV masked image back into BGR
    //imshow("Origional",BGR_maskedBoard); waitKey(0);

    // -----------------------------Contour-----------------------------
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    Mat contour_img = img.clone(); drawContours(contour_img, contours, -1, Scalar(0,255,0), 2); // Draw contours on the original image
    //imshow("Contours",contour_img); waitKey(0);

    sort(contours.begin(), contours.end(), compareContourAreas); // sort contours

    Rect GameBoard = boundingRect(contours[contours.size()-1]); // Find the rectangle encapsluating the largest contour
    //Mat croppedGameBoard = img(GameBoard); // Crop image to size of largest contour rectangle - outline of game board
    //Mat boarder_img = img.clone(); drawContours(boarder_img, contours,contours.size()-1, Scalar( 0, 255, 0 ), 2 ); // Draw the largest contour using largest stored index.
    //imshow("Boarder",boarder_img); waitKey(0); // Display the largest contour = boarder of the game 

    // Remove elements in counters array that aren't the counters
    // - Lower bound removes noise
    // - Upper bound removes the boarder
    for (int i = 0; i < contours.size(); i++) {
        float area = contourArea(contours[i]); // Calculate area of the contour
        if (area < 8000 || area > 12000) {
            contours.erase(contours.begin() + i); // Remove out of range element
            i--; // Removed element so all elements have shifted back, so need to research the previous element; therfore, decrease i
        }
    }

    // --Further Processing--
    // if (contours.size() == 42) {// No further processing;} else {// Do circularity check to remove non circular contours;}

    Mat counterContour_img = img.clone(); drawContours(counterContour_img, contours, -1, Scalar(0,255,0), 2); // Draw counter contours on the original image
    //imshow("Counter Contours",counterContour_img); waitKey(0);

    Point2f centre;
    float radius;
    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle(contours[i],centre,radius);
        cout << i << ": Centre: " << centre << endl;
    }

    //imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/CA_CountersContoured.jpg",counterContour_img);

    return 0;
}

bool sortYCoord(const Vec3f &a, const Vec3f &b) {
    return a[1] < b[1];
}
bool sortXCoord(const Vec3f &a, const Vec3f &b) {
    return a[0] < b[0];
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
