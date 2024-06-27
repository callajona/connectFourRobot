// Simple code to demonstrate that OpenCV is working
// Must download an image and copy its path and enter it into the image path variable

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string imagePath = "/enter/your/path/here"; // Must change this 
    
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Could not read the image: " << imagePath << std::endl;
        return 1;
    }
    
    cv::imshow("Display Window", image);
    cv::waitKey(0);
    return 0;
}
