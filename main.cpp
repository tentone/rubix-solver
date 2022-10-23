#include <iostream>

#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"

int main()
{
	// Test::test();

	Cube cube = Cube();
	cube.debug();
	cube.move(CubeMove::F, CubeMoveDirection::CW);
	cube.move(CubeMove::U, CubeMoveDirection::CW);
	cube.move(CubeMove::M, CubeMoveDirection::CW);
	cube.move(CubeMove::R, CubeMoveDirection::CCW);
	cube.move(CubeMove::M, CubeMoveDirection::CCW);
	cube.move(CubeMove::D, CubeMoveDirection::CW);

	// std::cout << cube.toString() << std::endl;

	// cube.scramble();
   
	bool sol = CubeSolver::solveBF(cube);
	std::cout << sol << std::endl;
}

