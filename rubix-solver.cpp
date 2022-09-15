#include <iostream>
#include "cube.cpp"

int main()
{
    Cube cube = Cube();
    cube.move(CubeMove::U, CubeMoveDirection::CW);

    std::cout << cube.toString();
}

