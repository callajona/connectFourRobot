#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <algorithm> // For sort

using namespace std;
using namespace cv;

// Prototypes
Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness);

int main(int argc, char** argv) 
{
    // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourWhiteBackground.jpg");
    
    // Check if image is found
    if (!img.data) {
        cout << "No image data\n" << endl;
        return -1;
    }

    // -------------------------------------------- Image Processing--------------------------------------------
    
    // ----------------------------- Resize Image -----------------------------
    float SF = 0.5; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor

    // ----------------------------- Change Brightness and Contrast -----------------------------
    Mat BrighterImg = BrightnessAndContrast(img,1,0);

    // ----------------------------- Crop Image -----------------------------
    int w = img.size().width; // W: 2304
    int h = img.size().height; // H: 1296
    //img = img(Range(0.05*h,0.85*h), Range(0.15*w,0.75*w)); // Crop image
    
    // ----------------------------- Colour Chnage and Blur -----------------------------
    Mat gray, gray_blur; // Create Mat variables for storing the images
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    GaussianBlur(gray, gray_blur, Size(9, 9), 2, 2 ); // Blur the image

    // ----------------------------- Edge Detection -----------------------------
    Mat edge;
    Canny(gray_blur,edge,125,150,3,true); // Edge detection
    imshow("Edge detection",edge); waitKey(0);

    // ----------------------------- Contour Detection -----------------------------
    // detect the contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // draw contours on the original image
    Mat image_copy = img.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    imshow("Contours",image_copy); waitKey(0);

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

    // ----------------------------- Circle Detection -----------------------------
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
