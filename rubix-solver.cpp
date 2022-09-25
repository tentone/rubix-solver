#include <iostream>
#include "cube.cpp"

int main()
{
    Cube cube = Cube();
    cube.move(CubeMove::U, CubeMoveDirection::CW);
    cube.move(CubeMove::M, CubeMoveDirection::CW);
    cube.move(CubeMove::D, CubeMoveDirection::CW);

    std::cout << cube.toString();
}

