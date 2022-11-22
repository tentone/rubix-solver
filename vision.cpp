#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "config.cpp"
#include "cube.cpp"
#include "utils.cpp"

/**
 * Color ranges in HSV used for filtering the color of the cube faces.
 */
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
	cv::Scalar(0, 75, 50),
	cv::Scalar(12, 150, 255),

	// Red
	cv::Scalar(0, 0, 50),
	cv::Scalar(12, 75, 255),
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
	int color = 7;

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

/**
 * Compares quads based on their position.
 * 
 * Used to sort quads by their coordinates from top to bottom, left to right.
 */
bool quad_sort(Quad a, Quad b) {
	const float tolerance = 30.0;

	cv::Point a_p = a.center();
	cv::Point b_p = b.center();

	if (abs(a_p.y - b_p.y) > tolerance) {
		return a_p.y < b_p.y;
	}

    return a_p.x < b_p.x;
}


enum State {
	IDLE,
	SCANNING,
	SOLVE
};

cv::Scalar CubeColors[] = {
	cv::Scalar(255, 255, 255), // W
	cv::Scalar(255, 0, 0), // B
	cv::Scalar(0, 255, 255), // Y
	cv::Scalar(0, 255, 0), // G
	cv::Scalar(0, 140, 230), // O
	cv::Scalar(0, 0, 255), // R
	cv::Scalar(0, 0, 0)
};


class Vision {
	public:
		/**
		 * Cube object to store result in.
		 */
		Cube cube;

		/**
		 * Solution to the cube. Needs to be manually triggered.
		 */
		CubeSolution sol;

		/**
		 * State of the vision system.
		 */
		State state;

		/**
		 * Number of iteration used to solve the cube.
		 */
		int iterations = 5;

		Vision() {
			this->cube = Cube();
			this->sol = CubeSolution();
			this->state = SCANNING;
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

					// Check colors of each quad
					for (int i = 0; i < quads.size(); i++)
					{
						Quad quad = quads[i];

						// Create mask of the square
						cv::Mat mask = cv::Mat::zeros(image.rows, image.cols, CV_8U);
						std::vector<std::vector<cv::Point>> contours;
						contours.push_back(quad.points);
						cv::drawContours(mask, contours, 0, cv::Scalar(255, 255, 255), cv::FILLED);

						// Mask draw
						// cv::imshow("Mask", mask);
						
						// Filter square using mask
						cv::Mat square;
						image.copyTo(square, mask);

						// Area of the square
						int area_square = cv::countNonZero(mask);
						
						// Amount of area to accept the color
						const float area_threshold = 0.6;

						for (int j = 0; j < 6; j++) {
							cv::Mat filter = this->segment_colors(square, ranges[j * 2], ranges[j * 2 + 1]);
							int area_filter = cv::countNonZero(filter);

							float ratio = float(area_filter) / float(area_square);

							// Accept color
							if (ratio > area_threshold) {
								quads[i].color = j;
								break;
							}
						}
					}

					// Check if the color of all cells was detected
					bool complete = true;
					for (int j = 0; j < 9; j++) {
						if (quads[j].color == 7) {
							complete = false;
						}
					}

					// Fill information of the cube face
					if (complete) {
						// Center color define the face to fill info
						int face = quads[4].color;

						// Copy data from the detected model to the cube face
						for (int j = 0; j < 9; j++) {
							this->cube.cube[face][j] = quads[j].color;
						}
					}

					// Debug the cube in action
					this->draw_quads(image, quads);
				}

				// Info
				this->draw_info(image);
				this->draw_cube(image);

				// Display image
				cv::imshow(window, image);
				int key = cv::waitKey(1);

				// S
				if (key == 115) {
					CubeSolution sol = CubeSolver::solveBF(cube, this->iterations);
					if (sol.solved) {
						this->sol = sol;
					}
					std::cout << sol.toString() << std::endl;
				}
				// R
				else if (key == 114) {this->cube.clear();}
				// +
				else if (key == 171) {this->iterations++;}
				// -
				else if (key == 173) {this->iterations--;}
				// ESC
				else if (key == 27) {break;}

				// std::cout << key << std::endl;
			}

			cv::destroyWindow(window);
		}
		
		/**
		 * Draw textual information to screen.
		 */
		void draw_info(cv::Mat src) {
			cv::Scalar color = cv::Scalar(0, 0, 255);

			cv::putText(src, "State: " + std::to_string(this->state), cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);
			cv::putText(src, "R to Reset", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);
			cv::putText(src, "S to Solve", cv::Point(10, 60), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);

			if (this->sol.solved) {
				cv::putText(src, this->sol.solved ? "Cube solved" : "Scan cube and press S to solve", cv::Point(10, src.rows - 30), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);
			}
			
		}


		/**
		 * Draw the cube into the mat.
		 */
		void draw_cube(cv::Mat src) {
			int size = 30;
			int space = size * 3;
			int x = 130;
			int y = 100;

			draw_cube_face(src, x, y, 4, size);
			draw_cube_face(src, x, y + space, 0, size);
			draw_cube_face(src, x, y + space * 2, 5, size);
			draw_cube_face(src, x, y + space * 3, 2, size);

			draw_cube_face(src, x - space, y + space, 1, size);
			draw_cube_face(src, x + space, y + space, 3, size);
		}
	
		/**
		 * Draw the cube face into the matrix
		 */
		void draw_cube_face(cv::Mat src, int off_x, int off_y, int idx, int s = 20) {
			
			for (int i = 0; i < 9; i++) {
				int x = i % 3;
				int y = (i / 3);

				cv::Point2i offset = cv::Point2i(off_x, off_y);

				cv::Scalar color = CubeColors[this->cube.cube[idx][i]];
				cv::rectangle(src, cv::Rect(x * s + offset.x, y * s + offset.y, s, s), color, -1);
				
			}
		}


		/**
		 * Method to debug the quads detected in the image.
		 * 
		 * Creates a mat with debug information and displays it to the screen.
		 */
		void draw_quads(cv::Mat src, std::vector<Quad> quads) {
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
				cv::line(src, quad[0], quad[1], colors[0]);
				cv::line(src, quad[1], quad[2], colors[1]);
				cv::line(src, quad[2], quad[3], colors[2]);
				cv::line(src, quad[3], quad[0], colors[3]);

				// Index
				cv::putText(src, std::to_string(i), quads[i].center(), cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);

				// Color
				cv::putText(src, CubeFaceColors[quads[i].color], quad[0], cv::FONT_HERSHEY_DUPLEX, 0.5, color, 1, false);
			}
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
		cv::Mat segment_colors(cv::Mat src, cv::Scalar min, cv::Scalar max) {
			const bool debug = false;

			// Indicate if the close operation should be applied
			const bool close_op = false;

			// Convert to HLS
			cv::Mat hls;
			cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

			cv::Mat mask;
			cv::inRange(hls, min, max, mask);

			if (close_op) {
				cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7), cv::Point(3, 3));
				cv::dilate(mask, mask, element);
				cv::erode(mask, mask, element);
			}
	

			if (debug) {
				cv::Mat rgb;
				src.copyTo(rgb, mask);
				cv::imshow("Filtered", rgb);
			}

			return mask;
		}
};
