#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Vision {
    public:
        static void start() {
            // Read the image file
            cv::Mat image = cv::imread("./readme/notation.png");

            // Check for failure
            if (image.empty()) 
            {
                std::cout << "Could not open or find the image" << std::endl;
                return;
            }
            std::string window = "Rubix";
            cv::namedWindow(window);

            cv::imshow(window, image);

            cv::waitKey(0);
            cv::destroyWindow(window);
        }

        static void quads() {
            
        }
};