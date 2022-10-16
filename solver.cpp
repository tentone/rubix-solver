#pragma once

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
	static bool solveBF(Cube cube, int depth=1000, std::list<CubeStep> solution = {}) {
		if (cube.solved()) {
			return true;
		}

		if (depth == 0) {
			return false;
		}

		// All possible moves
		for (int m = 0; m < 9; m++) {
			// CCW / CW
			for (int d = 0; d < 2; d++) {
				CubeStep step;
				step.direction = d;
				step.move = m;
				solution.push_back(step);

				std::list<CubeStep> sol(solution);

				Cube c = Cube(cube);
				c.move(m, d);
		
				if (solveBF(c, depth - 1, solution)) {
					return true;
				}
			}
		}

		return false;
	}
};