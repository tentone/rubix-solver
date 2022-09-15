#include <string>

#include <cstdlib>

/**
 * List of possible cube movements.
 * 
 * Based on the list https://jperm.net/3x3/moves.
 * 
 * Simplified without double movements or cube rotations.
 */
enum class CubeMove {
	U, // Up Face
	D, // Down Face
	R, // Right Face
	L, // Left Face
	F, // Front Face 
	B, // Back Face
	M, // Middle Row (Y)
	E, // Middle Row (X)
	S  // Middle Row (Z)
};

/**
 * Direction at wich the cube movements can be performed
 * 
 * Can be clockwise or conuter-clockwise
 */
enum class CubeMoveDirection {
	CW,
	CCW
};

/**
 * Index of the faces in the cube bidimensional array.
 */
class CubeFace {
public:
	static const int F = 0; // Front
	static const int R = 1; // Right
	static const int B = 2; // Back
	static const int L = 3; // Left
	static const int U = 4; // Up
	static const int D = 5; // Down
};


class Cube {
public: 
	/**
	 * Data of the rubix cube stored as bidimensional array
	 * 
	 * The array is composed of 6 faces x 9 squares for each face.
	 */
	int cube[6][9];
	
	/**
	 * Create a new rubix cube by default each face is filled with the same color.

	 */
	Cube() {
		this->clear();
	}

	/**
	 * Clear the cube and reset to its original configuration.
	 */
	void clear() {
		for (int f = 0; f < 6; f++) {
			for (int s = 0; s < 9; s++) {
				this->cube[f][s] = f;
			}
		}
	}

	/**
	 * Apply a move to the cube.
	 */
	void move(CubeMove move, CubeMoveDirection direction) {
		if (move == CubeMove::U) {
			if (direction == CubeMoveDirection::CW) {
				int temp[3];
			
				// Rotate rows
				std::copy(this->cube[CubeFace::F], this->cube[CubeFace::F] + 3, temp);
				std::copy(this->cube[CubeFace::L] + 0, this->cube[CubeFace::L] + 3, this->cube[CubeFace::F]);
				std::copy(this->cube[CubeFace::B] + 0 , this->cube[CubeFace::B] + 3, this->cube[CubeFace::L]);
				std::copy(this->cube[CubeFace::R] + 0, this->cube[CubeFace::R] + 3, this->cube[CubeFace::B]);
				std::copy(temp + 0, temp + 3, this->cube[CubeFace::R]);

				// Rotate up face
				// TODO
			} else if (direction == CubeMoveDirection::CCW) {

			}
		}
	}




	/**
	 * Randomize the rubix cube.
	 * 
	 * This has to be done trought random movement in the cube.
	 * 
	 * Its not possible to simply generate random values since the edges on the cube cannot be physically swapped.
	 * 
	 * @param steps - How many scrambling steps to perform.
	 */
	void scramble(int steps) {
		// TODO <ADD CODE HERE>
	}

	/**
	 * Create a string that represents the 6 faces of the cube in a readable format.
	 * 
	 * Usefull to print the cube state into the terminal.
	 */
	std::string toString() {
		std::string out = "";

		
		for (int f = 0; f < 6; f++) {
			out += "F" + std::to_string(f) + "\n[";
			for (int s = 0; s < 9; s++) {
		
				out += std::to_string(this->cube[f][s]);

				if (s < 8) {
					out += ", ";
				}

				if ((s + 1) % 3 == 0 && s < 8) {
					out += "\n";
				}
			}
			out += "]\n";
		}

		return out;

	}

};