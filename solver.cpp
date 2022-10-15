#include <string>

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <list>

#include "cube.cpp"

/**
 * Cube solution step structure that contains the move and direction.
 */
struct CubeStep {
	int move;
	int direction;
};

/**
 * The cube solver implements method to solve the rubix cube problem.
 * 
 * Returns a list of cube moves to solve the cube.
 */
class CubeSolver {
public:

	/**
	 * Solve the cube with brute force method.
	 * 
	 * Attempt all possible move combinations up to certain depth.
	 * 
	 * If no solution is found for the cube, the code will trow an exception.
	 */
	static std::list<CubeStep> solveBF(Cube cube, int depth=100, std::list<CubeStep> solution = {}) {
		if (cube.solved()) {
			return solution;
		}

		// All possible moves
		for (int m = 0; m < 9; m++) {
			// Both directions
			for (int d = 0; d < 2; d++) {
				CubeStep step;
				step.direction = d;
				step.move = m;
				solution.push_back(step);

				Cube c = Cube(cube);
				c.move(m, d);
				solveBF(c, depth - 1, solution);
			}
		}

		return solution;
	}
};