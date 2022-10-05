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
	 * Rotate face cells for specific indexes
	 */
	void rotateFaceCells(int face_a, int face_b, int face_c, int face_d, int idx_a[3], int idx_b[3], int idx_c[3], int idx_d[3]) {
		int temp[3] = { 0, 0, 0 };
		int idx_temp[3] = {0, 1, 2};

		copyRowData(this->cube[face_a], temp, idx_a, idx_temp);
		copyRowData(this->cube[face_b], this->cube[face_a], idx_b, idx_a);
		copyRowData(this->cube[face_c], this->cube[face_b], idx_c, idx_b);
		copyRowData(this->cube[face_d], this->cube[face_c], idx_d, idx_c);
		copyRowData(temp, this->cube[face_d], idx_temp, idx_d);
	}

	/**
	 * Rotate a row around the cube.
	 *
	 * The row crosses 4 different faces of the cube.
	 */
	void rotateRow(int face_a, int face_b, int face_c, int face_d, int idx[3], CubeMoveDirection direction) {
		if (direction == CubeMoveDirection::CW) {
			rotateFaceCells(face_a, face_b, face_c, face_d, idx, idx, idx, idx);
		}
		else if (direction == CubeMoveDirection::CCW) {
			rotateFaceCells(face_d, face_c, face_b, face_a, idx, idx, idx, idx);
		}
	}

	/**
	 * Rotate a face of the cube.
	 * 
	 * The face is rotated around itself.
	 */
	void rotateFace(int face, CubeMoveDirection direction) {
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
	void move(CubeMove move, CubeMoveDirection direction) {
		// Up
		if (move == CubeMove::U) {
			int idx[3] = { 0, 1, 2 };

			// Rotate rows
			this->rotateRow(CubeFace::F, CubeFace::L, CubeFace::B, CubeFace::R, idx, direction);

			// Rotate faces
			this->rotateFace(CubeFace::U, direction);
			this->rotateFace(CubeFace::D, direction);
		}
		// Middle Y
		else if (move == CubeMove::M) {
			int idx[3] = { 3, 4, 5 };

			// Rotate rows
			this->rotateRow(CubeFace::F, CubeFace::L, CubeFace::B, CubeFace::R, idx, direction);
		}
		// Down
		else if (move == CubeMove::D) {
			int idx[3] = { 6, 7, 8 };

			// Rotate rows
			this->rotateRow(CubeFace::F, CubeFace::L, CubeFace::B, CubeFace::R, idx, direction);

			// Rotate faces
			this->rotateFace(CubeFace::U, direction);
			this->rotateFace(CubeFace::D, direction);
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