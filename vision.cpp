#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "config.cpp"
#include "cube.cpp"
#include "utils.cpp"


const cv::Scalar ranges[12] = {
	// White
	cv::Scalar(0, 0, 7),
	cv::Scalar(255, 255, 35),

	// Blue
	cv::Scalar(93, 0, 50),
	cv::Scalar(112, 255, 255),

	// Yellow
	cv::Scalar(26, 0, 50),
	cv::Scalar(40, 255, 255),

	// Green
	cv::Scalar(58, 0, 50),
	cv::Scalar(72, 255, 255),

	// Orange
	cv::Scalar(0, 55, 50),
	cv::Scalar(12, 80, 255),

	// Red
	cv::Scalar(0, 23, 50),
	cv::Scalar(12, 55, 255),
};



class Vision {
	public:


		/**
		 * Cube object to store result in.
		 */
		Cube cube;

		Vision() {
			this->cube = Cube();
		}

		/**
		 * @brief Start the vision module to detect cube.
		 */
		void detect() {
			cv::VideoCapture cap(CONFIG_CAMERA);
			if (!cap.isOpened())
			{
				std::cout << "Error accessing camera." << std::endl;
				return;
			}

			// Read the image file
			cv::Mat image;

			// Create window
			std::string window = "Rubix";
			cv::namedWindow(window);

			while (true) {
				// Capture image camera
				cap >> image;

				// Check for failure
				if (image.empty()) 
				{
					std::cout << "Could not open or find the image" << std::endl;
					return;
				}


				// Filter image based on color
				this->segmentColor(image);

				// Detect quads
				this->quads(image);

				// cv::imshow(window, image);
				int key = cv::waitKey(1);
				if (key == 27) {
					break;
				}
			}

			cv::destroyWindow(window);
		}

		/**
		 * @brief Detect quadrilaters in the image.
		 */
		void quads(cv::Mat src) {
			// Convert to grayscale
			cv::Mat gray;
			cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

			// Use Canny instead of threshold to catch squares with gradient shading
			cv::Mat bw;
			cv::Canny(gray, bw, 0, 50, 7);

			// cv::imshow("Gray", gray);
			// cv::imshow("BW", bw);

			// Find contours
			std::vector<std::vector<cv::Point>> contours;
			cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

			// Output matrix for debug
			cv::Mat dst = src.clone();

			for (int i = 0; i < contours.size(); i++)
			{
				std::vector<cv::Point> contour = contours[i];

				const int min_area = 200;
				const int max_area = 100000000;

				// Approximated points 
				std::vector<cv::Point> approx;
				
				// Approximate contour with accuracy proportional to the contour perimeter
				cv::approxPolyDP(contour, approx, cv::arcLength(contours[i], true) * 0.05, true);

				if (approx.size() == 4)
				{
					// Skip objects by size
					int area = cv::contourArea(approx);
					if (area < min_area || area > max_area) {
						continue;
					}
					
					// Skip non-convex objects 
					if (!cv::isContourConvex(approx)) {
						continue;
					}

					// Length of all edges
					std::vector<double> length;
					for (int j = 1; j < 4; j++) {
						length.push_back(cv::norm(approx[j] - approx[j - 1]));
					}

					// Similar sized edges
					// TODO

					// Get the cosines of all corners
					std::vector<double> cos;
					for (int j = 2; j <= 4; j++) {
						cos.push_back(angle(approx[j%4], approx[j-2], approx[j-1]));
					}

					// Filter by the angle of the corners (should be close to 90 deg)
					std::sort(cos.begin(), cos.end());
					double mincos = cos.front();
					double maxcos = cos.back();
					if (mincos < -0.3 && maxcos > 0.3) {
						continue;	
					}

					cv::Scalar color = cv::Scalar(255, 255,255);

					// Draw
					cv::line(dst, approx[0], approx[1], color);
					cv::line(dst, approx[1], approx[2], color);
					cv::line(dst, approx[2], approx[3], color);
					cv::line(dst, approx[3], approx[0], color);
				}
			}

			cv::imshow("Quads", dst);
		}


		/**
		 * Segment image based on rubix cube face colors.
		 */
		void segmentColor(cv::Mat src) {
			// Convert to HLS
			cv::Mat hls;
			cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

			cv::Mat mask;
			cv::inRange(hls, ranges[0], ranges[1], mask);

			cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7), cv::Point(3, 3));
			cv::dilate(mask, mask, element);
			cv::erode(mask, mask, element);

			// cv::bitwise_and(src, src, rgb, mask);

			cv::Mat rgb;
			src.copyTo(rgb, mask);

			cv::imshow("Filtered", rgb);
			
		}


};