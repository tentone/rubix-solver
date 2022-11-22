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
 * @brief Auxiliary method to convert a boolean value to string.
 * 
 * @param b 
 * @return const std::string 
 */
const std::string bool_cast(const bool b) {
    return b ? "true" : "false";
}

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

	std::string toString() {
		std::string out = "Solved: " + bool_cast(solved) + " | ";

		out += "Steps: ";
		for (std::list<CubeStep>::iterator s = this->steps.begin(); s != this->steps.end(); s++) {
			out += CubeMoveNames[s->move] + " (" + CubeMoveDirectionNames[s->direction] + "), ";
		}

		return out;
	}
};