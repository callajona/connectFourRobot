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
vector<Point2f> orderPoints(vector<Point2f> pts);
Mat fourPointTransform(const Mat& image, vector<Point2f> pts);

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

    // --------------------------------------------------------------------------------
    // Define the four points
    vector<Point2f> pts;
    pts.push_back(Point2f(522, 252));
    pts.push_back(Point2f(1462, 274));
    pts.push_back(Point2f(1450, 836));
    pts.push_back(Point2f(1448, 977));

    // Apply the four point transform to obtain a top-down view of the image
    Mat warped = fourPointTransform(img, pts);

    // Display the original and warped images
    imshow("Original Image", img);
    imshow("Warped Image", warped);
    waitKey(0);

    // --------------------------------------------------------------------------------

    
    Mat gray, gray_blur, edge; // Create Mat variables for storing the images
    Mat brighter_img = BrightnessAndContrast(img,3,0); // Increase the Contrast
    cvtColor(brighter_img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    GaussianBlur(gray, gray_blur, Size(9, 9), 2, 2 ); // Blur the image
    Canny(gray_blur,edge,125,150,3,true); // Edge detection
    imshow("Edge detection",edge); waitKey(0);
    
    // Circle Detection
    vector<Vec3f> circles;
    HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,55,59);
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
        cout << i << ": (" << x << "," << y << ")\n"; // Display Coordiatnes
        //cout << i << " - Radius = " <<  radius << endl;
        
        Point center(x,y); // Calculate centre point 

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
        else if (red > 170 && blue < 100 && green > 160) {
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
    namedWindow( "Output", 1 );
    imshow( "Output", img );
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

vector<Point2f> orderPoints(vector<Point2f> pts) {
    // Initialize a list of coordinates that will be ordered
    // such that the first entry is the top-left, the second
    // entry is the top-right, the third is the bottom-right,
    // and the fourth is the bottom-left
    vector<Point2f> rect(4);
    
    // The top-left point will have the smallest sum, whereas
    // the bottom-right point will have the largest sum
    vector<float> s;
    for (int i = 0; i < pts.size(); i++) {
        s.push_back(pts[i].x + pts[i].y);
    }
    rect[0] = pts[min_element(s.begin(), s.end()) - s.begin()];
    rect[2] = pts[max_element(s.begin(), s.end()) - s.begin()];

    // Now, compute the difference between the points
    // The top-right point will have the smallest difference,
    // whereas the bottom-left will have the largest difference
    vector<float> diff;
    for (int i = 0; i < pts.size(); i++) {
        diff.push_back(pts[i].y - pts[i].x);
    }
    rect[1] = pts[min_element(diff.begin(), diff.end()) - diff.begin()];
    rect[3] = pts[max_element(diff.begin(), diff.end()) - diff.begin()];

    return rect;
}

Mat fourPointTransform(const Mat& image, vector<Point2f> pts) {
    // Obtain a consistent order of the points and unpack them
    // individually
    vector<Point2f> rect = orderPoints(pts);
    Point2f tl = rect[0];
    Point2f tr = rect[1];
    Point2f br = rect[2];
    Point2f bl = rect[3];

    // Compute the width of the new image, which will be the
    // maximum distance between bottom-right and bottom-left
    // x-coordinates or the top-right and top-left x-coordinates
    float widthA = norm(br - bl);
    float widthB = norm(tr - tl);
    int maxWidth = max(int(widthA), int(widthB));

    // Compute the height of the new image, which will be the
    // maximum distance between the top-right and bottom-right
    // y-coordinates or the top-left and bottom-left y-coordinates
    float heightA = norm(tr - br);
    float heightB = norm(tl - bl);
    int maxHeight = max(int(heightA), int(heightB));

    // Now that we have the dimensions of the new image, construct
    // the set of destination points to obtain a "birds eye view"
    // (i.e. top-down view) of the image
    vector<Point2f> dst;
    dst.push_back(Point2f(0, 0));
    dst.push_back(Point2f(maxWidth - 1, 0));
    dst.push_back(Point2f(maxWidth - 1, maxHeight - 1));
    dst.push_back(Point2f(0, maxHeight - 1));

    // Compute the perspective transform matrix and then apply it
    Mat M = getPerspectiveTransform(rect, dst);
    Mat warped;
    warpPerspective(image, warped, M, Size(maxWidth, maxHeight));

    return warped;
}
