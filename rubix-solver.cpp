#include <iostream>
#include "cube.cpp"

int main()
{
    Cube cube = Cube();
    
    //cube.move(CubeMove::U, CubeMoveDirection::CW);
    //cube.move(CubeMove::M, CubeMoveDirection::CW);

    //cube.move(CubeMove::D, CubeMoveDirection::CW);
    //cube.move(CubeMove::D, CubeMoveDirection::CW);

    //cube.move(CubeMove::L, CubeMoveDirection::CW);

    //cube.move(CubeMove::F, CubeMoveDirection::CW);
    //cube.move(CubeMove::F, CubeMoveDirection::CW);


    cube.scramble();

    std::cout << cube.toString();
}

