#include <iostream>


#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"
#include "vision.cpp"

int main()
{
	Vision vision = Vision();
	vision.detect();

	// Test::test();

	Cube cube = Cube();
	// cube.scramble();
	cube.move(CubeMove::F, CubeMoveDirection::CW);
	cube.move(CubeMove::U, CubeMoveDirection::CW);
	cube.move(CubeMove::M, CubeMoveDirection::CW);
	cube.move(CubeMove::R, CubeMoveDirection::CCW);
	cube.move(CubeMove::M, CubeMoveDirection::CCW);
	cube.move(CubeMove::D, CubeMoveDirection::CW);

	std::cout << "Cube Read\n" << cube.toString() << std::endl;
   
	CubeSolution sol = CubeSolver::solveBF(cube, 10);
	std::cout << sol.toString() << std::endl;

	
}

