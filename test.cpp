#pragma once

#include <iostream>

#include "cube.cpp"
#include "solver.cpp"

class Test {
public:
    /**
     * Randomly generate a cube and try to solve it.
     */
    static bool random(int n = 5) {
        Cube cube = Cube();
        cube.scramble(n);

        CubeSolution sol = CubeSolver::solveBF(cube, n);

        return sol.solved;
    }

    /**
     * Test ensures that are moves can be performed in CW and then CCW to reset the cube.
     */
    static bool movecwccw() {
        Cube cube = Cube();

        for (int i = 0; i < 9; i++) {
            cube.move(i, CubeMoveDirection::CW);
        }

        for (int i = 8; i >= 0; i--) {
            cube.move(i, CubeMoveDirection::CCW);
        }

        return cube.solved();
    }

    /**
     * Front move to test face rotation.
     */
    static bool rotateFace() {
        Cube cube = Cube();
        cube.debug();
        cube.move(CubeMove::F, CubeMoveDirection::CW);

        int f[9] = { 6, 3, 0,
                     7, 4, 1,
                     8, 5, 2};

        for (int i = 0; i < 9; i++) {
            if (f[i] != cube.cube[CubeFace::F][i]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Run all tests defined here and print to console.
     */
    static void test()
    {
        std::cout << "Test Cube CW/CCW Moves: " << Test::movecwccw() << std::endl;
        std::cout << "Test Cube face rotation: " << Test::rotateFace() << std::endl;
        std::cout << "Test Cube random: " << Test::random() << std::endl;
    }
};
