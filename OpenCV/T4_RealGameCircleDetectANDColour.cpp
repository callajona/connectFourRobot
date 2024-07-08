#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>

using namespace std;
using namespace cv;

// rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour2.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png

int main(int argc, char** argv)
{
    // Import image into code - ENSURE PATH IS CHANGED BEFORE USE
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg");
    Mat gray;

    // Colour Detection Variables
    Vec3b pixelColour;
    int blue;
    int green;
    int red;
    
    // Check if image is found
    if (!img.data)
    {
        cout << "No image data\n" << endl;
        return -1;
    }

    // Image Processing
    float scaleFactor = 0.5;
    int up_width = img.cols * scaleFactor;
    int up_height = img.rows * scaleFactor;
    resize(img, img, Size(up_width, up_height),0,0, INTER_LINEAR);
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert img to grayscale
    //GaussianBlur( gray, gray, Size(9, 9), 2, 2 ); // smooth it, otherwise a lot of false circles may be detected

    // Circle Detection
    vector<Vec3f> circles;
    HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,52,57);
    //HoughCirles(in,out,TYPE,resulution SF, no. circles per width, 300, 30, min radius, max radius)
    // variables that worked for the gemerated image: HoughCircles(gray, circles, HOUGH_GRADIENT,2, gray.rows/9, 300, 110);
    // Variables that worked for real image: HoughCircles(gray, circles,HOUGH_GRADIENT,1.55,gray.rows/10,50,40,52,57);
    cout << "Number of Circles: " << circles.size() << endl; // Output number of circles

    for(size_t i = 0; i < circles.size(); i++)
    {
        // Get Coordinates from Circles vector
        float x = cvRound(circles[i][0]);
        float y = cvRound(circles[i][1]);
        //cout << "(" << x << "," << y << ")\n"; // Display Coordiatnes
        
        // Calculate centre point and find radius
        Point center(x,y);
        int radius = cvRound(circles[i][2]);

        // Fing BGR values at coordinates and split into indiviual varaiables
        pixelColour = img.at<Vec3b>(y, x); 
        blue = pixelColour[0];
        green = pixelColour[1];
        red = pixelColour[2];
        //cout << "B: " << blue << "  G: " << green << "  R: " << red << endl; // Display colours
        
        // --- Draw circles on image ---
        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 ); // draw the circle outline

        // Draw centre cirlce - Change colour depending on colour detected
        if (red > 150 && blue < 200 && green < 200) {
            circle( img, center, 10, Scalar(0,0,255), -1, 8, 0 ); // draw the circle center - red when counter is red
        }
        else if (red > 200 && blue < 200 && green > 200) {
            circle( img, center, 10, Scalar(0,255,255), -1, 8, 0 ); // draw the circle center - yellow when counter is yellow
        }
        else {
            circle( img, center, 10, Scalar(0,0,0), -1, 8, 0 ); // draw the circle center - Black when no counter
        }        
    }

    // Create window to display image
    namedWindow( "circles", 1 );
    imshow( "circles", img );

    // Save image
    //imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/circleDectect_RealGame_ColouredCentres1.jpg",img); 

    waitKey(0);
    return 0;
}
