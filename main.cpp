#pragma once 

#include <iostream>

#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"

int main()
{
    Test::test();
    

    CubeStep steps[] = {
        CubeStep(CubeMove::F, CubeMoveDirection::CW),
        CubeStep(CubeMove::U, CubeMoveDirection::CW),
        CubeStep(CubeMove::U, CubeMoveDirection::CCW),
        CubeStep(CubeMove::M, CubeMoveDirection::CW),
        CubeStep(CubeMove::B, CubeMoveDirection::CW),
        CubeStep(CubeMove::F, CubeMoveDirection::CCW),
    };


    Cube cube = Cube();
    cube.debug();
    std::cout << "----------------------\n" << cube.toString() << std::endl;
    //for (int i = 0; i < sizeof(steps); i++) {

    //    cube.move(steps[i].move, steps[i].direction);
    //    
    //    std::cout << "----------------------\n" << cube.toString() << std::endl;
    //}

    // bool sol = CubeSolver::solveBF(cube);
    // std::cout << sol << std::endl;
}

