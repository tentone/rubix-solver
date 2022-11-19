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


/**
 * Represents a quad detected in the image. 
 */
struct Quad {
	/**
	 * List of points that compose the quad.
	 * 
	 * Must always be composed of four points
	 */
	std::vector<cv::Point> points;

	/**
	 * Color of the quad.
	 * 
	 * Based on the rubix cube color faces
	 */
	int color;

	/**
	 * Get the center of the quad.
	 */
	cv::Point center() {
		cv::Point center = cv::Point(0.0, 0.0);

		for (int i = 0; i < this->points.size(); i++)
		{
			center += this->points[i];
		}

		center /= float(this->points.size());
		
		return center;
	}

};

// Compares two intervals according to starting times.
bool quad_sort(Quad a, Quad b) {
	cv::Point a_p = a.center();
	cv::Point b_p = b.center();

	if (abs(a_p.y - b_p.y) > 30.0) {
		return a_p.y < b_p.y;
	}

    return a_p.x < b_p.x;
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

				// Detect quads
				std::vector<Quad> quads = this->quads(image);

	


				// Figure which quad belongs to witch face
				if (quads.size() == 9) {
					// Sort based on x, y positions
					std::sort(quads.begin(), quads.end(), quad_sort);


			this->debug_quads(image, quads);

					// Check colors of each quad
					for (int i = 0; i < quads.size(); i++)
					{
						Quad quad = quads[i];
						
						
					}
				}


				// Filter image based on color
				this->segment_colors(image);

				// cv::imshow(window, image);
				int key = cv::waitKey(1);
				if (key == 27) {
					break;
				}
			}

			cv::destroyWindow(window);
		}


		/**
		 * Method to debug the quads detected in the image.
		 * 
		 * Creates a mat with debug information and displays it to the screen.
		 */
		void debug_quads(cv::Mat src, std::vector<Quad> quads) {
			cv::Mat dst = src.clone();

			cv::Scalar color = cv::Scalar(255, 255, 255);
			cv::Scalar colors[] = {
				cv::Scalar(255, 0, 0),
				cv::Scalar(0, 255, 0),
				cv::Scalar(0, 0, 255),
				cv::Scalar(255, 255, 0)
			};

			for (int i = 0; i < quads.size(); i++)
			{
				std::vector<cv::Point> quad = quads[i].points;

				// Draw Quad
				cv::line(dst, quad[0], quad[1], colors[0]);
				cv::line(dst, quad[1], quad[2], colors[1]);
				cv::line(dst, quad[2], quad[3], colors[2]);
				cv::line(dst, quad[3], quad[0], colors[3]);

				// Center index
				cv::putText(dst, std::to_string(i), quads[i].center(), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);
			}

			cv::imshow("Quads", dst);
		}

		/**
		 * @brief Detect quadrilaters in the image.
		 */
		std::vector<Quad> quads(cv::Mat src) {
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

			// Quads detected
			std::vector<Quad> quads;

			for (int i = 0; i < contours.size(); i++)
			{
				std::vector<cv::Point> contour = contours[i];

				const int min_area = 2000;
				const int max_area = 5000;

				// Approximated points 
				std::vector<cv::Point> approx;
				
				// Approximate contour with accuracy proportional to the contour perimeter
				cv::approxPolyDP(contour, approx, cv::arcLength(contours[i], true) * 0.04, true);

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

					// Similar sized edges (square)
					const double max_diff = 100.0;
					double max = 0.0;
					for (int i = 0; i < 4; i++) {
						for (int j = i; j < 4; j++) {
							double diff = abs(length[j] - length[i]);
							if (max < diff) {
								max = diff;
							}
						}
					}

					if(max > max_diff) {
						continue;
					}

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

					Quad quad = Quad();
					quad.points = approx;

					// It is a valid quad add to the list~
					quads.push_back(quad);
				}
			}

			return quads;
		}


		/**
		 * Segment image based on rubix cube face colors.
		 */
		void segment_colors(cv::Mat src) {
			const bool debug = true;

			// Convert to HLS
			cv::Mat hls;
			cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

			cv::Mat mask;
			cv::inRange(hls, ranges[4], ranges[5], mask);

			cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7), cv::Point(3, 3));
			cv::dilate(mask, mask, element);
			cv::erode(mask, mask, element);

			// cv::bitwise_and(src, src, rgb, mask);

			cv::Mat rgb;
			src.copyTo(rgb, mask);

			if (debug) {
				cv::imshow("Filtered", rgb);
			}
		}


};