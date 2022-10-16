#pragma once 

#include <iostream>

#include "solver.cpp"
#include "test.cpp"
#include "cube.cpp"

int main()
{
    Test::test();
    
    Cube cube = Cube();
    cube.move(CubeMove::F, CubeMoveDirection::CW);
    cube.move(CubeMove::F, CubeMoveDirection::CW);
    cube.scramble();
    std::cout << cube.toString() << std::endl;


    //bool sol = CubeSolver::solveBF(cube);
    //std::cout << sol << std::endl;

    //// NOK
    //cube.move(CubeMove::D, CubeMoveDirection::CW);

    // //NOK L
    // cube.move(CubeMove::L, CubeMoveDirection::CW);




    // 

  

    // 
}

