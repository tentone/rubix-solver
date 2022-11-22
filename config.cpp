#pragma once

// Video source
#define CONFIG_CAMERA "http://192.168.0.40:8080/video"

// GUI window name
#define CONFIG_WINDOW "Rubix"

// Area % required to consider for color test
#define CONFIG_AREA_THRESHOLD 0.6

// Quad approximation tolerance
#define CONFIG_APPROX_TOL 0.04
// Minimum cube cell area
#define CONFIG_MIN_AREA 1500
// Maximum cube cell area
#define CONFIG_MAX_AREA 7000
// Minimum difference between cell edges to consider a square
#define CONFIG_EDGE_DIFF 100.0
// Minimum angle cosine to consider the quad as being rect (close to 0 to have 90 deg)
#define CONFIG_MIN_COSINE -0.3
// Maximum angle cosine to consider the quad as being rect (close to 0 to have 90 deg)
#define CONFIG_MAX_COSINE 0.3

// Indicate if the color segmentation should apply close operation before area test
#define CONFIG_SEGMENT_CLOSE false

// Height tolerance applied when sorting quads
#define CONFIG_SORT_Y_TOL 30.0

// HSL color Ranges
#define WHITE_MIN cv::Scalar(0, 0, 7)
#define WHITE_MAX cv::Scalar(255, 255, 35)

#define BLUE_MIN cv::Scalar(93, 0, 50)
#define BLUE_MAX cv::Scalar(112, 255, 255)

#define YELLOW_MIN cv::Scalar(26, 0, 50)
#define YELLOW_MAX cv::Scalar(40, 255, 255)

#define GREEN_MIN cv::Scalar(58, 0, 50)
#define GREEN_MAX cv::Scalar(72, 255, 255)

#define ORANGE_MIN cv::Scalar(0, 75, 50)
#define ORANGE_MAX cv::Scalar(12, 150, 255)

#define RED_MIN cv::Scalar(0, 0, 50)
#define RED_MAX cv::Scalar(12, 75, 255)
