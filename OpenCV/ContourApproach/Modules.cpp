// Different funtions required for the approach to work

// -------------------------------------------- Import Image --------------------------------------------
    Mat img = imread("/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/connectFour_cameraDistanceTest2.jpg");
    
    // Check if image is found
    if (!img.data) {
        cout << "No image data\n" << endl;
        return -1;
    }

    // ---------------------------------------------------------------------------------------- Image Processing----------------------------------------------------------------------------------------
    
    // --------------------------------------------Resize Image--------------------------------------------
    float SF = 0.5; // Scaling factor
    resize(img, img, Size(), SF, SF, INTER_LINEAR); // Resize image by scaling factor
    //imshow("Image",img); waitKey(0);

    // -------------------------------------------- Masking the Board --------------------------------------------
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
    bitwise_and(HSV_img, HSV_img, maskedBoard, mask); // Use mask to remove everything else from the image
    //imshow("Mask",mask); waitKey(0); // Display the mask

    Mat BGR_maskedBoard; cvtColor(maskedBoard,BGR_maskedBoard,COLOR_HSV2BGR); // Convert the HSV masked image back into BGR
    //imshow("Origional",BGR_maskedBoard); waitKey(0);

    // --------------------------------------------Contour--------------------------------------------
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Detect the contours

    Mat contour_img = img.clone(); drawContours(contour_img, contours, -1, Scalar(0,255,0), 2); // Draw contours on the original image
    //imshow("Contours",contour_img); waitKey(0);

    sort(contours.begin(), contours.end(), compareContourAreas); // sort contours

    // --------------------------------------------Finding Camera Distance--------------------------------------------
    // Find the distance betweent the board and the camera
    // - Approximate the shape of the board contour to find key points around board perimeter (including top corners)
    // - Isolate the top corners
    // - Calculate pixel distance between points - Pythagoras
    // - Use equation with Focal length to calculate distnace to camera
    // For more accurate results - Calibrate the focal length (Measure pixel width and a range of distances and average/alter focal length based on pixel width)
    // Currently ~2% tolerance

    // Board width = 260mm

    // Approximate board shape 
    vector<Point> approxBoard, corners;
    approxPolyDP(contours[contours.size()-1],approxBoard,100,true); // Low accuracy to get the few most important points
    
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

    float focalLength = 1787.62; // FocalLength = (pixWidth * cameraDistance) / boardWidth --> Calibrated with: F = (968.292*480)/260 [mm]
    float cameraDist = (260 * focalLength) / pixBoardWidth;
    cout << "Camera Distance: " << cameraDist << "mm" << endl;

    // Draw found corners onto image
    circle( img, TLcorner, 10, Scalar(255,0,0), 3, 8, 0 ); // draw Top-Left corner
    circle( img, TRcorner, 10, Scalar(255,0,0), 3, 8, 0 ); // draw Top-Right corner
    imshow("Corners Identified",img); waitKey(0); // Display corners

    // --------------------------------------------Masking out the counter colours--------------------------------------------
    Scalar redMinHSV = Scalar(170,51,77); // [H: 0, S: 0.2, V: 0.3]
    Scalar redMaxHSV = Scalar(179,255,255); // [H: 12, S: 1, V: 1]

    Scalar yelMinHSV = Scalar(18,51,102); // [H: 36, S: 0.2, V: 0.4]
    Scalar yelMaxHSV = Scalar(40,255,255); // [H: 74, S: 1, V: 1]

    // Mask Red
    Mat redMask, redImg;
    inRange(HSV_img,redMinHSV,redMaxHSV,redMask); // Create red mask
    bitwise_and(HSV_img, HSV_img, redImg, redMask); // Apply red mask
    Mat BGR_redImg; cvtColor(redImg,BGR_redImg,COLOR_HSV2BGR); // convert masked image to BGR to be displayed
    //imshow("Red Mask",BGR_redImg); waitKey(0); // 

    // Mask Yelow
    Mat yelMask, yelImg;
    inRange(HSV_img,yelMinHSV,yelMaxHSV,yelMask); // create yellow mask
    bitwise_and(HSV_img, HSV_img, yelImg, yelMask); // Apply yellow mask
    Mat BGR_yelImg; cvtColor(yelImg,BGR_yelImg,COLOR_HSV2BGR); // Convert masked image to BGR to be displayed
    //imshow("Yellow Mask",BGR_yelImg); waitKey(0); // Display just yellow circles

    // Combine Masks
    Mat redYelMask, outputImg;
    bitwise_or(yelMask,redMask,redYelMask); // Combine two masks
    bitwise_and(HSV_img, HSV_img, outputImg, redYelMask); // Mask image using combined mask
    Mat BGR_bothMasks; cvtColor(outputImg,BGR_bothMasks,COLOR_HSV2BGR); // Convert masked image to BGR to be diplayed
    imshow("Both Masks",BGR_bothMasks); waitKey(0);

    
