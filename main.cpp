#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"

int main()
{
	// Read the image file
	cv::Mat image = cv::imread("./readme/notation.png");

	// Check for failure
	if (image.empty()) 
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	std::string window = "Rubix";
	cv::namedWindow(window);

	cv::imshow(window, image);

	cv::waitKey(0);
	cv::destroyWindow(window);

	// Test::test();

	Cube cube = Cube();
	cube.debug();
	cube.move(CubeMove::F, CubeMoveDirection::CW);
	cube.move(CubeMove::U, CubeMoveDirection::CW);
	// cube.move(CubeMove::M, CubeMoveDirection::CW);
	// cube.move(CubeMove::R, CubeMoveDirection::CCW);
	// cube.move(CubeMove::M, CubeMoveDirection::CCW);
	// cube.move(CubeMove::D, CubeMoveDirection::CW);

	// std::cout << cube.toString() << std::endl;

	// cube.scramble();
   
	CubeSolution sol = CubeSolver::solveBF(cube);
	std::cout << sol.toString() << std::endl;

	
}

