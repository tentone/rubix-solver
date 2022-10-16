#pragma once 

#include <iostream>

#include "cube.cpp"
#include "solver.cpp"
#include "test.cpp"

int main()
{
    Test::test();

    Cube cube = Cube();
    
    cube.move(CubeMove::U, CubeMoveDirection::CW);
    cube.move(CubeMove::M, CubeMoveDirection::CW);
    cube.move(CubeMove::D, CubeMoveDirection::CW);
    
    cube.move(CubeMove::D, CubeMoveDirection::CCW);
    cube.move(CubeMove::M, CubeMoveDirection::CCW);
    cube.move(CubeMove::U, CubeMoveDirection::CCW);

    // NOK
    //cube.move(CubeMove::D, CubeMoveDirection::CW);

    // NOK L
    // cube.move(CubeMove::L, CubeMoveDirection::CW);

    //cube.move(CubeMove::F, CubeMoveDirection::CW);
    //cube.move(CubeMove::F, CubeMoveDirection::CW);


    // cube.scramble();

    // bool sol = CubeSolver::solveBF(cube);
    // std::cout << sol << std::endl;

    std::cout << cube.toString() << std::endl;
}

