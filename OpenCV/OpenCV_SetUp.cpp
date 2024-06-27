#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string imagePath = "/home/project/projects/ConnectFourProject/OpenCV/my_cpp_project/src/Images/";
    
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Could not read the image: " << imagePath << std::endl;
        return 1;
    }
    
    cv::imshow("Display window", image);
    cv::waitKey(0);
    return 0;
}
