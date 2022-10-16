#pragma once

#include <iostream>

#include "cube.cpp"
#include "solver.cpp"

class Test {
public:
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
     * Run all tests defined here and print to console.
     */
    static void test()
    {
        std::cout << "Test Cube CW/CCW Moves: " << Test::movecwccw() << std::endl;
    }
};
