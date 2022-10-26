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
	/**
	 * @brief Move to be applied to the cube
	 */
	int move;

	/**
	 * @brief Diretion of the move to apply.
	 */
	int direction;

	CubeStep(int move, int direction) {
		this->move = move;
		this->direction = direction;
	}
};


/**
 * @brief 
 */
struct CubeSolution {
	/**
	 * @brief List with the steps that compose this solution.
	 */
	std::list<CubeStep> steps = {};

	/**
	 * @brief Flag to indicate if this composes a valid solution.
	 */
	bool solved = false;

	CubeSolution() {}

	CubeSolution(std::list<CubeStep> steps) {
		this->steps = std::list<CubeStep>(steps);
	}
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
	static CubeSolution solveBF(Cube cube, int depth=6, CubeSolution solution = CubeSolution()) {
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
				// Add step to list
				CubeStep step = CubeStep(m, d);
				
				// Clone solution list
				CubeSolution sol = CubeSolution(solution);
				sol.steps.push_back(step);

				// Clone cube
				Cube c = Cube(&cube);
				c.move(m, d);
		
				sol = CubeSolver::solveBF(cube, depth - 1, sol);
				if (sol.solved) {
					return sol;
				}
			}
		}

		return solution;
	}
};