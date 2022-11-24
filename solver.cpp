#pragma once

#include <string>

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <list>

#include "cube.cpp"
#include "solution.cpp"

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
	static CubeSolution solveBF(Cube cube, int depth=3, CubeSolution solution = CubeSolution()) {
		if (cube.solved()) {

			solution.solved = true;
			return solution;
		}

		if (depth <= 0) {
			return solution;
		}

		// All possible moves
		for (int m = 0; m < 9; m++) {
			// CCW / CW
			for (int d = 0; d < 2; d++) {
				// If its the same move but in oposite direction skip
				if (solution.steps.size() > 0) {
					CubeStep last = solution.steps.back();
					if (last.move == m && last.direction != d) {
						continue;
					}
				} 

				// Clone solution and add step to list
				CubeSolution sol = CubeSolution(solution);
				sol.steps.push_back(CubeStep(m, d));

				// Clone cube and apply move
				Cube c = Cube(&cube);
				c.move(m, d);
		
				sol = CubeSolver::solveBF(c, depth - 1, sol);
				if (sol.solved) {
					return sol;
				}
			}
		}

		return solution;
	}
};