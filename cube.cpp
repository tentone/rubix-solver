#pragma once

#include <string>

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <list>

/**
 * List of possible cube movements.
 * 
 * Based on the list https://jperm.net/3x3/moves.
 * 
 * Simplified without double movements or cube rotations.
 */
class CubeMove {
public:
	static const int U = 0; // Up Face
	static const int D = 1; // Down Face
	static const int R = 2; // Right Face
	static const int L = 3; // Left Face
	static const int F = 4; // Front Face 
	static const int B = 5; // Back Face
	static const int M = 6; // Middle Row (Y)
	static const int E = 7; // Middle Row (X)
	static const int S = 8; // Middle Row (Z)
};

/**
 * Direction at wich the cube movements can be performed
 * 
 * Can be clockwise or conuter-clockwise
 */
class CubeMoveDirection {
public:
	static const int CW = 0; // Clock-Wise
	static const int CCW = 1; // Counter Clock-Wise
};

/**
 * Index of the faces in the cube bidimensional array.
 * 
 * Each face values is stored as array.
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
	 * 
	 * Each face indexes are sorted by row.
	 * 
	 * [0, 1, 2,
	 *  3, 4, 5,
	 *  6, 7, 8]
	 */
	int cube[6][9];
	
	/**
	 * Create a new rubix cube by default each face is filled with the same color.
	 */
	Cube() {
		this->clear();
	}

	/**
	 * Copy constructor do duplicate cube values.
	 */
	Cube(const Cube& rhs) {
		for (int f = 0; f < 6; f++) {
			for (int s = 0; s < 9; s++) {
				this->cube[f][s] = rhs.cube[f][s];
			}
		}
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
	 * Copy data from face A to face B for specific indexes.
	 * 
	 * From origin to destination in specific order
	 */
	void copyRowData(int* face_ori, int* face_dest, int idx_ori[3], int idx_dest[3]) {
		for (int i = 0; i < 3; i++) {
			face_dest[idx_dest[i]] = face_ori[idx_ori[i]];
		}
	}

	/**
	 * Rotate face cells of a row for eith specific indexex for each storage.
	 * 
	 * A <- B <- C <- D
	 *
	 * The row crosses 4 different faces of the cube.
	 * 
	 * Receives a list of faces by order and indexes to store the cells by order for each row.
	 */
	void rotateRow(int face_a, int face_b, int face_c, int face_d, int idx_a[3], int idx_b[3], int idx_c[3], int idx_d[3], int direction) {
		int temp[3] = { 0, 0, 0 };
		int idx_temp[3] = {0, 1, 2};

		if (direction == CubeMoveDirection::CCW) {
			copyRowData(this->cube[face_a], temp, idx_a, idx_temp);
			copyRowData(this->cube[face_b], this->cube[face_a], idx_b, idx_a);
			copyRowData(this->cube[face_c], this->cube[face_b], idx_c, idx_b);
			copyRowData(this->cube[face_d], this->cube[face_c], idx_d, idx_c);
			copyRowData(temp, this->cube[face_d], idx_temp, idx_d);
		}
		else if (direction == CubeMoveDirection::CW)
		{
			copyRowData(this->cube[face_d], temp, idx_d, idx_temp);
			copyRowData(this->cube[face_c], this->cube[face_d], idx_c, idx_d);
			copyRowData(this->cube[face_b], this->cube[face_c], idx_b, idx_c);
			copyRowData(this->cube[face_a], this->cube[face_b], idx_a, idx_b);
			copyRowData(temp, this->cube[face_a], idx_temp, idx_a);
		}
	}

	/**
	 * Rotate a row around the cube.
	 *
	 * A <- B <- C <- D
	 * 
	 * The row crosses 4 different faces of the cube.
	 */
	void rotateRow(int face_a, int face_b, int face_c, int face_d, int idx[3], int direction) {
		rotateRow(face_a, face_b, face_c, face_d, idx, idx, idx, idx, direction);
	}

	/**
	 * Rotate a face of the cube.
	 * 
	 * The face is rotated around itself.
	 */
	void rotateFace(int face, int direction) {
		int* cface = this->cube[face];

		if (direction == CubeMoveDirection::CW)
		{
			int a = cface[0];
			cface[0] = cface[3];
			cface[3] = cface[6];
			cface[6] = cface[7];
			cface[7] = cface[8];
			cface[8] = cface[5];
			cface[5] = cface[2];
			cface[2] = cface[1];
			cface[1] = a;
		}
		else
		{
			int a = cface[0];
			cface[0] = cface[1];
			cface[1] = cface[2];
			cface[2] = cface[5];
			cface[5] = cface[8];
			cface[8] = cface[7];
			cface[7] = cface[6];
			cface[6] = cface[3];
			cface[3] = a;
		}

	}

	/**
	 * Apply a move to the cube.
	 */
	void move(int move, int direction) {
		// Up Face
		if (move == CubeMove::U) {
			int idx[3] = { 0, 1, 2 };

			this->rotateRow(CubeFace::F, CubeFace::L, CubeFace::B, CubeFace::R, idx, direction);
			this->rotateFace(CubeFace::U, direction);;
		}
		// Down Face
		else if (move == CubeMove::D) {
			int idx[3] = { 6, 7, 8 };

			this->rotateRow(CubeFace::F, CubeFace::R, CubeFace::B, CubeFace::L, idx, direction);
			this->rotateFace(CubeFace::D, direction);
		}
		// Right Face
		else if (move == CubeMove::R) {
			int idx[4][3] = {
				{ 2, 5, 8 },
				{ 2, 5, 8 },
				{ 6, 3, 0 },
				{ 2, 5, 8 }
			};

			this->rotateRow(CubeFace::F, CubeFace::U, CubeFace::B, CubeFace::D, idx[0], idx[1], idx[2], idx[3], direction);
			this->rotateFace(CubeFace::R, direction);
		}
		// Left face
		else if (move == CubeMove::L) {
			int idx[4][3] = {
				{ 0, 3, 6 },
				{ 0, 3, 6 },
				{ 8, 5, 2 },
				{ 0, 3, 6 }
			};

			this->rotateRow(CubeFace::F, CubeFace::D, CubeFace::B, CubeFace::U, idx[0], idx[1], idx[2], idx[3], direction);
			this->rotateFace(CubeFace::L, direction);
		}
		// Front face
		else if (move == CubeMove::F) {
			int idx[4][3] = {
				{ 2, 5, 8 },
				{ 8, 7, 6 },
				{ 6, 3, 0 },
				{ 0, 1, 2 }
			};

			this->rotateRow(CubeFace::L, CubeFace::U, CubeFace::R, CubeFace::D, idx[0], idx[1], idx[2], idx[3], direction);
			this->rotateFace(CubeFace::F, direction);
		}
		// Back face
		else if (move == CubeMove::B) {
			int idx[4][3] = {
				{ 0, 3, 6 },
				{ 2, 1, 0 },
				{ 8, 5, 2 },
				{ 6, 7, 8 }
			};

			this->rotateRow(CubeFace::L, CubeFace::U, CubeFace::R, CubeFace::D, idx[0], idx[1], idx[2], idx[3], direction);
			this->rotateFace(CubeFace::B, direction);
		}
		// Middle E
		else if (move == CubeMove::E) {
			int idx[3] = { 3, 4, 5 };

			this->rotateRow(CubeFace::F, CubeFace::L, CubeFace::B, CubeFace::R, idx, direction);
		}
		// Middle M
		else if (move == CubeMove::M) {
			int idx[4][3] = {
				{ 1, 4, 7 },
				{ 1, 4, 7 },
				{ 1, 4, 7 },
				{ 7, 4, 1 }
			};

			this->rotateRow(CubeFace::U, CubeFace::F, CubeFace::D, CubeFace::B, idx[0], idx[1], idx[2], idx[3], direction);
		}
		// Middle Z
		else if (move == CubeMove::S) {
			int idx[4][3] = {
				{ 3, 4, 5 },
				{ 1, 4, 7 },
				{ 5, 4, 3 },
				{ 7, 4, 1 }
			};

			this->rotateRow(CubeFace::U, CubeFace::R, CubeFace::D, CubeFace::L, idx[0], idx[1], idx[2], idx[3], direction);
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
	void scramble(int steps = 1000) {
		srand(time(0));

		for (int i = 0; i < steps; i++) {
			int direction = rand() % 1;
			int action = rand() % 5;
			this->move(action, direction);
		}
	}

	/**
	 * Check if the cube is solved.
	 */
	bool solved() {
		for (int f = 0; f < 6; f++) {
			int val = this->cube[f][0];
			for (int s = 1; s < 9; s++) {
				if (val != this->cube[f][s]) {
					return false;
				}
			}
		}

		return true;
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