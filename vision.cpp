#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "config.cpp"
#include "cube.cpp"

/**
 * Helper function to find a cosine of angle between vectors from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

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

				// Convert to HLS
				cv::Mat hls;
				cv::cvtColor(image, hls, cv::COLOR_BGR2HLS);

				this->quads(image);

				cv::imshow(window, image);
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
			// cv::dilate(bw, bw, cv::Mat(), cv::Point(-1, -1));

			cv::imshow("Graw", gray);
			cv::imshow("BW", bw);

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

					// Length of all sides
					std::vector<double> length;
					for (int j = 1; j < 4; j++) {
						length.push_back(cv::norm(approx[j] - approx[j - 1]));
					}

					// Get the cosines of all corners
					std::vector<double> cos;
					for (int j = 2; j <= 4; j++) {
						cos.push_back(angle(approx[j%4], approx[j-2], approx[j-1]));
					}

					// Sort ascending the cosine values
					std::sort(cos.begin(), cos.end());

					// Get the lowest and the highest cosine
					double mincos = cos.front();
					double maxcos = cos.back();

					// Use the degrees obtained above and the number of vertices to determine the shape of the contour
					if (mincos >= -0.1 && maxcos <= 0.4) {
						cv::Scalar color = cv::Scalar(255, 255,255);

						// Draw
						cv::line(dst, approx[0], approx[1], color);
						cv::line(dst, approx[1], approx[2], color);
						cv::line(dst, approx[2], approx[3], color);
						cv::line(dst, approx[3], approx[0], color);
					}
				}
			}

			cv::imshow("Quads", dst);
		}

		void segmentColor() {


			// Yellow

			// Blue

			// Red

			// Orange

			// Green

			// White
		}

};