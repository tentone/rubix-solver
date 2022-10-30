#include <iostream>


#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"
#include "vision.cpp"

int main()
{
	Vision::start();

	Test::test();

	Cube cube = Cube();
	cube.move(CubeMove::F, CubeMoveDirection::CW);
	cube.move(CubeMove::U, CubeMoveDirection::CW);
	cube.move(CubeMove::M, CubeMoveDirection::CW);
	cube.move(CubeMove::R, CubeMoveDirection::CCW);
	cube.move(CubeMove::M, CubeMoveDirection::CCW);
	cube.move(CubeMove::D, CubeMoveDirection::CW);

	// std::cout << cube.toString() << std::endl;

	// cube.scramble();
   
	CubeSolution sol = CubeSolver::solveBF(cube, 10);
	std::cout << sol.toString() << std::endl;

	
}

