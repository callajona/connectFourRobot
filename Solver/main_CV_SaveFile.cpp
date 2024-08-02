#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <algorithm> // For sort

#include "chrono" // For time tracking

//using namespace std;
using namespace cv;

// --- Prorotypes ---
bool sortYCoord(const Point &a, const Point &b);
bool sortXCoord(const Point &a, const Point &b);
bool compareContourAreas(std::vector<Point> contour1, std::vector<Point> contour2); // comparison function object

bool imageChangeDetection(Mat img1, Mat img2);
int calibrateImgScaleFactor(Mat img);

void GS_Analysis_Demo(Mat img); // Game State Analyser - best for demonstration
void GS_Analysis(Mat img); // Game State analyser - optimised for performance

int GS_diffCheck();
int col_diff(); // Checks which column has changed

void GS_Check();
void GS_display();
bool GS_errorCheck(); // Checks for missread empty slots (e.g. counter above a slot)

Mat BrightnessAndContrast(Mat inputImg, float contrast, int brightness);

// --- Global Variables ---
int gameState[42];
int lastKnown_gameState[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Get image: rpicam-jpeg --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour9.jpg
// Image1: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour.jpg
// Image2: /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/move4.png
  
	//GS_Analysis_Demo(img); // connectFour6.jpg - best for demo

int main(int argc, char** argv) 
{
	bool win = false;

	while (win == false) { // Game Loop
		while (GS_diffCheck() == 0) {

			system("rpicam-still --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourImage.jpg --timeout 500"); // take picture
			system("clear");
			Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourImage.jpg");
			
			// Check if image is found
			if (!img.data) {
				std::cout << "No image data\n" << std::endl;
				return -1;
			}

			GS_Analysis(img);
			GS_display();
			GS_errorCheck();
  	}	
		
		// Check again
		system("rpicam-still --output /home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourImage.jpg --timeout 500"); // take picture
		system("clear");
		Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFourImage.jpg");
		
		// Check if image is found
		if (!img.data) {
			std::cout << "No image data\n" << std::endl;
			return -1;
		}

		GS_Analysis(img);
		GS_display();
		GS_errorCheck();

		if (GS_diffCheck() == 1) {
			std::cout << "Single Change detected" << std::endl;

      int col = col_diff();
      std::cout << "Change in Col " << col << std::endl;

			// Update last known game state
			for (int i = 0; i < 42; i++) {
				lastKnown_gameState[i] = gameState[i];
			}
		}
    else if (GS_diffCheck() > 1) {
      std::cerr << "Error - Too many changes" << std::endl;
    }
	}

  return 0;
}

/* Clock Function
  const auto start{std::chrono::steady_clock::now()}; // Start Clock
  const auto end{std::chrono::steady_clock::now()}; // end Clock
  const std::chrono::duration<double> elapsed_seconds{end - start};
  cout << "Duration Time: " << elapsed_seconds.count() << endl;
*/

bool sortYCoord(const Point &a, const Point &b) {
    return a.y < b.y;
}
bool sortXCoord(const Point &a, const Point &b) {
    return a.x < b.x;
}
bool compareContourAreas(std::vector<Point> contour1, std::vector<Point> contour2) {
    double i = fabs( contourArea(Mat(contour1)) );
    double j = fabs( contourArea(Mat(contour2)) );
    return ( i < j );
}

bool imageChangeDetection(Mat img1, Mat img2) {
    // Detect when there is a change in game state by comparing two images
    // Approach: 
    // - Mask out red and yellow contours
    // - Compare masks between two images
    // - Find major differences (detect contours --> detect contours of a certain size and circularity)
    // - If diffrences found: change in GS detected

    bool output = false; // Output remains false unless change is detected

    // --------------------------------------------Masking out the counter colours--------------------------------------------
    // Convert Images to HSV
    Mat img1_HSV, img2_HSV;
    cvtColor(img1,img1_HSV,COLOR_BGR2HSV); 
    cvtColor(img2,img2_HSV,COLOR_BGR2HSV);

    // Set Ranges
    Scalar redMinHSV = Scalar(170,51,77); // [H: 0, S: 0.2, V: 0.3]
    Scalar redMaxHSV = Scalar(179,255,255); // [H: 12, S: 1, V: 1]

    Scalar yelMinHSV = Scalar(18,51,102); // [H: 36, S: 0.2, V: 0.4]
    Scalar yelMaxHSV = Scalar(40,255,255); // [H: 74, S: 1, V: 1]

    Mat redMask, redImg, yelMask, yelImg, redYelMask1, redYelMask2; // Create images

    // ------------------Image 1------------------
    // Mask Red
    inRange(img1_HSV,redMinHSV,redMaxHSV,redMask); // Create red mask
    bitwise_and(img1_HSV, img1_HSV, redImg, redMask); // Apply red mask

    // Mask Yelow
    inRange(img1_HSV,yelMinHSV,yelMaxHSV,yelMask); // create yellow mask
    bitwise_and(img1_HSV, img1_HSV, yelImg, yelMask); // Apply yellow mask

    // Combine Masks
    bitwise_or(yelMask,redMask,redYelMask1); // Combine two masks
    //imshow("Mask", redYelMask1); waitKey(0);  

    // ------------------Image 2------------------
    // Mask Red
    inRange(img2_HSV,redMinHSV,redMaxHSV,redMask); // Create red mask
    bitwise_and(img2_HSV, img2_HSV, redImg, redMask); // Apply red mask

    // Mask Yelow
    inRange(img2_HSV,yelMinHSV,yelMaxHSV,yelMask); // create yellow mask
    bitwise_and(img2_HSV, img2_HSV, yelImg, yelMask); // Apply yellow mask

    // Combine Masks
    bitwise_or(yelMask,redMask,redYelMask2); // Combine two masks
    //imshow("Mask", redYelMask2); waitKey(0); 

    // ------------------Compare Masks------------------
    Mat outputMask;
    bitwise_xor(redYelMask1, redYelMask2, outputMask); // Compare masks 
    //imshow("Change Mask", outputMask); waitKey(0); 

    // -----------------------------Contour-----------------------------
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(outputMask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    Mat contour_img = img2.clone(); drawContours(contour_img, contours, -1, Scalar(0,255,0), 2); // Draw contours on the original image
    imshow("Contours",contour_img); waitKey(0);

    // -----------------------------Process the Contours-----------------------------
    // Look for contours of a specific area and circularity
 
    for (int i = 0; i < contours.size(); i++) {
        float area = contourArea(contours[i]); // Calculate area of the contour
        if (area > 1000 && area < 20000) {
            // Detect circular contours
            float perimeter = arcLength(contours[i], true); // Find perimeter of closed shape - true determines closed shape
            float area = contourArea(contours[i]); // Find area
            float circularity = 4*M_PI*(area /(perimeter*perimeter)); // Calculate a circularity

            if (circularity > 0.5) {
                output = true;
            }
        }
    }
    return output;

    /*
    // Code for main funtion

    Mat img1 = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour_changeTest_old.jpg");
    Mat img2 = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour_changeTest_new.jpg");
    
    // Check if image is found
    if (!img1.data) {
        cout << "No image 1 data\n" << endl;
        return -1;
    }
    if (!img2.data) {
        cout << "No image 2 data\n" << endl;
        return -1;
    }

    // -------------------------------------------- Image Processing--------------------------------------------
    
    // Resize Image
    float SF = 0.4; // Scaling factor
    resize(img1, img1, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor
    resize(img2, img2, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor
    //imshow("Image",img); waitKey(0);

    bool result = imageChangeDetection(img1,img2);

    if (result == true) {
        cout << "Change detected!" << endl;
    }
    else {
        cout << "No change Detected" << endl;
    }
    */
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

void GS_Analysis_Demo(Mat img) {
    // --------------------------------- Slower and less efficient Game State Analysis - displays each step ---------------------------------
    // Resize Image
    float SF = 0.4; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor
    imshow("Resized Image",img); waitKey(0);

    // --------------------------------------------- Masking the Board ---------------------------------------------
    // HSV = Hue, Saturation, Value
    // Hue - Colour [0-180] - Usally 360, but halfed to be stored in a byte
    // Saturation [0-255] - 0 = White, 255 = Full colour
    // Value [0-255] - 0 = Black, 255 = Full Colour

    Mat HSV_img;
    cvtColor(img,HSV_img,COLOR_BGR2HSV); // Convert Image to HSV space 
    
    // Set upper and lower bounds to mask
    Scalar minHSV = Scalar(107,85,60);
    Scalar maxHSV = Scalar(135,255,255);

    // Mask out the board
    Mat mask, maskedBoard;
    inRange(HSV_img, minHSV, maxHSV, mask); // Create board mask by checking if HSV img has pixel in specified range

    // Unnecessary steps - don't use the board Mask image, just the mask itself
    bitwise_and(HSV_img, HSV_img, maskedBoard, mask); // Use mask to remove everything else from the image
    imshow("Board Mask",mask); waitKey(0); // Display the mask

    Mat BGR_maskedBoard; cvtColor(maskedBoard,BGR_maskedBoard,COLOR_HSV2BGR); // Convert the HSV masked image back into BGR
    imshow("Board Masked Image",BGR_maskedBoard); waitKey(0); // Display masked out board

    // -----------------------------Contour-----------------------------
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    Mat contour_img = img.clone(); drawContours(contour_img, contours, -1, Scalar(0,255,0), 2); // Draw contours on the original image
    imshow("Contours Detected",contour_img); waitKey(0); // Display the contours

    sort(contours.begin(), contours.end(), compareContourAreas); // sort contours

    // -----------------------------Process the Contours-----------------------------
    // Remove elements in contours array that aren't the counters

    // Remove very large or very small contours
    // - Lower bound removes noise
    // - Upper bound removes the boarder
    for (int i = 0; i < contours.size(); i++) {
        float area = contourArea(contours[i]); // Calculate area of the contour
        std::cout << "Area = " << area << std::endl;
        if (area < 2000 || area > 20000) {
            contours.erase(contours.begin() + i); // Remove out of range element
            i--; // Decrease i to resarch index as element has been removed, so vector shifts back
        }
    }

    Mat counterContour_img = img.clone(); drawContours(counterContour_img, contours, -1, Scalar(0,255,0), 2); // Draw counter contours on the original image
    imshow("Removed Small and Large Contours",counterContour_img); waitKey(0);

    // Remove non circular contours
    float perimeter;
    float area; 
    float circularity;

    for (int i = 0; i < contours.size(); i++) {
        perimeter = arcLength(contours[i], true); // Find perimeter of closed shape - true determines closed shape
        area = contourArea(contours[i]); // Find area
        circularity = 4*M_PI*(area /(perimeter*perimeter)); // Calculate a circularity
        std::cout << i << ": " << circularity << std::endl;

        if (circularity < 0.5 || circularity > 1) {
            contours.erase(contours.begin() + i); // Remove out of range element
            //cout << "Circularity beign deleted: " << circularity << endl;
            i--; // Decrease i to resarch index as element has been removed, so vector shifts back
        }
    }
               
    Mat counterContour2_img = img.clone(); drawContours(counterContour2_img, contours, -1, Scalar(0,255,0), 2); // Draw counter contours on the original image
    imshow("Remove Non-Circular Contours",counterContour2_img); waitKey(0);

    if (contours.size() != 42) {
      std::cerr << "Error: Incorrect number of circles!" << std::endl;
    }

    // -----------------------------Circile Centre points-----------------------------
    // Find Centre points of contours using the Minimum Enclosing Circle function

    Point2f centre;
    float radius;
    std::vector<Point> centrePoints; // Vector to store centre points - Point data type to round centre values

    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle(contours[i],centre,radius); // Find the Minimum enclosing circle around the points - gives radius and centre point
        centrePoints.insert(centrePoints.begin() + i, centre); // add centre point to array
    }
    
    // Sort Circles based on centre points
    sort(centrePoints.begin(), centrePoints.end(), sortYCoord); // Sort all vectors based on y-coordinates (rows still scrambled: e.g. 3 2 1; 4 6 5; 9 7 8)
    for (int i = 0; i < 6; i++) {
        sort(centrePoints.begin() + (i*7), centrePoints.begin() +7 + (i*7),sortXCoord); // Sort each row
    }

    // Display the Fitted circles
    Mat fittedCircles = img.clone();
    for (int i = 0; i < centrePoints.size(); i++) {
        //cout << i << " - Centre: " << centrePoints[i] << endl; // output centre points to terminal

        // Draw the circles onto the image
        circle(fittedCircles, centrePoints[i], i, Scalar(0,0,0), -1, 8, 0 ); // draw the circle center
        circle(fittedCircles, centrePoints[i], radius, Scalar(255,0,0), 3, 8, 0 ); // draw the circle outline
    }
    imshow("Fitted and Sorted Circles", fittedCircles); waitKey(0);

    // -----------------------------Colour Detection-----------------------------
    // Detect colours in centre of circles
    
    Vec3b pixHSV;
    int H, S, V;

    Mat finalImg = img.clone();

    for (int i = 0; i < centrePoints.size(); i++) {
        pixHSV = HSV_img.at<Vec3b>(centrePoints[i]); // Get the HSV values at the centre points of the circle
        
        // Seperate pixel parameters
        H = pixHSV[0];
        S = pixHSV[1];
        V = pixHSV[2];

        // Detect Colours
        if (((H >= 170 && H <= 179) || (H >= 0 && H <= 12)) && (S >= 51 && S <=255) && (V >=77 && V <= 255)) {
            gameState[i] = 1;
            circle(finalImg, centrePoints[i], 5, Scalar(0,0,255), -1, 8, 0 ); // draw the centre point - red
        }
        else if ((H >= 18 && H <= 40) && (S >= 51 && S <=255) && (V >=102 && V <= 255)) {
            gameState[i] = 2;
            circle(finalImg, centrePoints[i], 5, Scalar(0,255,255), -1, 8, 0 ); // draw the centre point - Yellow
        }
        else {
            gameState[i] = 0;
            circle(finalImg, centrePoints[i], 5, Scalar(0,0,0), -1, 8, 0 ); // draw the centre point - Black
        }

        circle(finalImg, centrePoints[i], radius, Scalar(255,0,0), 3, 8, 0 ); // draw the outline circle
    }

    imshow("Colour Analysis", finalImg); waitKey(0); // Display final Image
}
void GS_Analysis(Mat img) {
    // --------------------------------- Faster and more efficient Game State Analysis - No display ---------------------------------
    // Resize Image
    float SF = 0.4; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor

    // --------------------------------------------- Masking the Board ---------------------------------------------
    // HSV = Hue, Saturation, Value
    // Hue - Colour [0-180] - Usally 360, but halfed to be stored in a byte
    // Saturation [0-255] - 0 = White, 255 = Full colour
    // Value [0-255] - 0 = Black, 255 = Full Colour

    Mat HSV_img;
    cvtColor(img,HSV_img,COLOR_BGR2HSV); // Convert Image to HSV space 
    
    // Set upper and lower bounds to mask
    Scalar minHSV = Scalar(107,85,60);
    Scalar maxHSV = Scalar(135,255,255);

    // Mask out the board
    Mat mask, maskedBoard;
    inRange(HSV_img, minHSV, maxHSV, mask); // Create board mask by checking if HSV img has pixel in specified range

    // -----------------------------Contour-----------------------------
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    // -----------------------------Process the Contours-----------------------------
    // Remove elements in contours array that aren't the counters

    // Remove very large or very small contours 
    // - Lower bound removes noise
    // - Upper bound removes the boarder
    // Remove non circular contours

    std::vector<std::vector<Point>> counterContours; // Vector to store the contours that are counters

    float area; 
    float perimeter;
    float circularity;

    int j = 0; // Index for the counter contour array

    for (int i = 0; i < contours.size(); i++) {
        area = contourArea(contours[i]); // Calculate area of the contour
        if (area > 1000 && area < 20000) { // If within certain area - process circularity
            
            perimeter = arcLength(contours[i], true); // Find perimeter of closed shape - true determines closed shape
            circularity = 4*M_PI*(area /(perimeter*perimeter)); // Calculate a circularity

            if (circularity > 0.5 && circularity < 1) {
                counterContours.insert(counterContours.begin() + j, contours[i]);
                j++;
            }   
        }
    }

    if (counterContours.size() != 42) { // Or j != 41
      std::cerr << "Error: Incorrect number of circles!" << std::endl;
    }

    // -----------------------------Circile Centre points-----------------------------
    // Find Centre points of contours using the Minimum Enclosing Circle function

    Point2f centre;
    float radius;
    std::vector<Point> centrePoints; // Vector to store centre points - Point data type to round centre values

    for (int i = 0; i < counterContours.size(); i++) {
        minEnclosingCircle(counterContours[i],centre,radius); // Find the Minimum enclosing circle around the points - gives radius and centre point
        centrePoints.insert(centrePoints.begin() + i, centre); // add centre point to array
    }
    
    // Sort Circles based on centre points
    sort(centrePoints.begin(), centrePoints.end(), sortYCoord); // Sort all vectors based on y-coordinates (rows still scrambled: e.g. 3 2 1; 4 6 5; 9 7 8)
    for (int i = 0; i < 6; i++) {
        sort(centrePoints.begin() + (i*7), centrePoints.begin() +7 + (i*7),sortXCoord); // Sort each row
    }

    // -----------------------------Colour Detection-----------------------------
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

}

int GS_diffCheck() {
	int diff = 0; // 0 = no change, 1 = change, > 1 == error

	for (int i = 0; i < 42; i++) {
    if (gameState[i] != lastKnown_gameState[i]) {
      diff++;
    }
  }

	return diff;
}
int col_diff() {
  int col;

  for (int i = 0; i < 42; i++) {
    if (gameState[i] != lastKnown_gameState[i]) {
      col = (i % 7) + 1;
      break;
    }
  }

  return col;
}

void GS_display() {
    // -----------------------------Display Game State in Terminal-----------------------------
    // Display Column Numbers
    for (int i = 1; i <= 7; i++) {
        std::cout << i << " "; // Display Column numbers
    }
    std::cout << "\n";

    // Display individual cells
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            std::cout << gameState[j+(i*7)] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
bool GS_errorCheck() {
    bool error = false;
    bool counterFound = false;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (counterFound == false) { // if counter not found in column
                if (gameState[j*7 + i] != 0) { // Check counter in new position
                    counterFound = true;
                }
            }
            else { // When counter has been found in column
                if(gameState[j*7 + i] == 0) { // Check for 0 below counter = obvious error 
                    error = true;
                    std::cout << "Error in Column: " << i + 1 << std::endl;
                    break;
                }
            }
        }
        counterFound = false; // Reset for each column
    }

    return error;
}

int calibrateImgScaleFactor(Mat img) {
    // Generate scale factor for a set of images
    // Approach:
    // - Mask the board
    // - Find the contours of the mask
    // - Approximate the largest contour (the boarder) to generate corner points
    // - Isolate the correct corner points (TL and TR)
    // - Find the distance between them (Pythagoras)
    // - Use this pixel width to calculate a scaling factor
    //      - Board width = 260mm
    //      - Counter Apature Diameter = 26mm
    //      - Divide BW by 10 --> Counter Apature Pixel Diameter
    //      - Use to find pixel diameter of counters --> Pixel area
    //      - Scale image to make area consistent

    // -------------------------------------------- Masking the Board --------------------------------------------
    Mat HSV_img;
    cvtColor(img,HSV_img,COLOR_BGR2HSV); // Convert Image to HSV space 
    
    // Set upper and lower bounds to mask
    Scalar minHSV = Scalar(107,85,60);
    Scalar maxHSV = Scalar(135,255,255);

    // Mask out the board
    Mat mask;
    inRange(HSV_img, minHSV, maxHSV, mask); // Create board mask by checking if HSV img has pixel in specified range

    // --------------------------------------------Contour Detection--------------------------------------------
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    // --------------------------------------------Find board Contour--------------------------------------------
    float largestArea = 0;
    int largestAreaIndex;

    // Compare all areas and save the index of the largest area
    for (int i = 0; i < contours.size(); i++) {
        if (contourArea(contours[i]) > largestArea) {
            largestArea = contourArea(contours[i]); // Save current contour area as largest area
            largestAreaIndex = i;
        }
    }

    // --------------------------------------------Find Pixel Width of the Board--------------------------------------------
    // - Approximate the shape of the board contour to find key points around board perimeter (including top corners)
    // - Isolate the top corners
    // - Calculate pixel distance between points - Pythagoras

    // Approximate board shape 
    std::vector<Point> approxBoard, corners;
    approxPolyDP(contours[largestAreaIndex],approxBoard,100,true); // Low accuracy to get the few most important points
    
    // Find two corner points
    // 1) Sort points based on height [Y-coordinates]
    // 2) Focus on highest (smallest value) ones - find difference between biggest and smallest, only look at top 10%
    // 3) Sort subset based on width [x-coordinates] - take the largset and smallest

    sort(approxBoard.begin(), approxBoard.end(),sortYCoord); // Sort points based on height
    int heightRange = approxBoard[approxBoard.size() - 1].y - approxBoard[0].y; // Find difference between highest and lowest points
    for (int i = 0; i < approxBoard.size(); i++) {
        if (approxBoard[i].y < ((0.1 * heightRange) + approxBoard[0].y)) { // Check if y coordiante is within 10% of height range from the highest coordinate
            corners.push_back(approxBoard[i]); // Add highest points to corners array
        }
    }
    sort(approxBoard.begin(), approxBoard.end(),sortXCoord); // Sort points based on width

    // Find distance between two points - Pythagoras
    Point TLcorner = corners[0];
    Point TRcorner = corners[corners.size() - 1];

    float pythag_w = abs((TRcorner.x - TLcorner.x)); // Work out the width of the triangle
    float pythag_h = abs((TRcorner.y - TLcorner.y)); // Work out the height of the triangle
    float pixBoardWidth = sqrt((pythag_w*pythag_w) + (pythag_h*pythag_h)); // Calculate the disntance between the corners using pythagoras

    float pixCounterArea = M_PI * ((pixBoardWidth / 10) / 2) * ((pixBoardWidth / 10) / 2);

    std::cout << "Board Pixel Width: " << pixBoardWidth << " pixels" << std::endl;
    std::cout << "Counter Pixel Area: " << pixCounterArea << " pixels" << std::endl;

    // Calcualted: 41,000
    // Lowest: 30,000
    // Highest: 44,000
    // Tolerance: 30
    
    /*
    cout << "Pixel Width: " << pixBoardWidth << " pixels" << endl;

    // Draw found corners onto image
    circle(img, TLcorner, 10, Scalar(255,0,0), 3, 8, 0 ); // draw Top-Left corner
    circle(img, TRcorner, 10, Scalar(255,0,0), 3, 8, 0 ); // draw Top-Right corner
    imshow("Corners Identified",img); waitKey(0); // Display corners
    //imwrite("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/SFcalibration_CornersDetect.jpg",img);
    */

    return 0;
}

/* Camera Capture
    // Open the default camera with the V4L2 backend
    VideoCapture cap(0, CAP_V4L2);

    // Check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR: Could not open the camera" << endl;
        return -1;
    } 

    // Set the desired frame width and height
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);

    // Capture a frame
    Mat frame;
    cap >> frame;

    // Check if we succeeded
    if (frame.empty()) {
        cerr << "ERROR: Could not capture a frame" << endl;
        return -1;
    }
 
    // Save the frame to an image file
    imshow("Captured Image", frame); waitKey(0);
    //imwrite("captured_image.jpg", frame);

    cout << "Image captured and saved to 'captured_image.jpg'" << endl;

    // Release the camera
    cap.release();

    */
