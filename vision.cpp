#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Vision {
    public:
        /**
         * @brief Start the vision module to detect cube.
         */
        static void start() {
            cv::VideoCapture cap("http://192.168.0.40:8080/video");
            if (!cap.isOpened())
            {
                std::cout << "Error accessing camera." << std::endl;
                return;
            }

            // Read the image file
            cv::Mat image; // = cv::imread("./readme/notation.png");

            while (true) {
                // Capture image camera
                cap >> image;

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

                return;
            }
        }

        /**
         * @brief Detect quadrilaters in the image.
         */
        static void quads() {
            
        }
};