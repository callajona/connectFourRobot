#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png");
    Mat gray;

    // Check if image is found
    if (!img.data)
    {
        cout << "No image data\n" << endl;
        return -1;
    }

    // Image Processing
    int scaleFactor = 4;
    int up_width = img.rows * (scaleFactor+1);
    int up_height = img.cols * scaleFactor;
    resize(img, img, Size(up_width, up_height), INTER_LINEAR);
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    //GaussianBlur( gray, gray, Size(9, 9), 2, 2 ); // smooth it, otherwise a lot of false circles may be detected

    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT,2, gray.rows/9, 300, 110);
    // gray = input image
    // circles = output vector for positional information
    // HOUGH_GRADIENT = type
    // 2 = resolution
    // gray.rows/9 = Mindistance betweem circles. Calculates width of image and divides by value greater than 7 (due to 7 columns therefore 7 circles across)

    cout << circles.size() << endl; // Output number of circles

    for(size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    namedWindow( "circles", 1 );
    imshow( "circles", img );

    imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/circleDetectOutput.jpg",img); 

    waitKey(0);
    return 0;
}
