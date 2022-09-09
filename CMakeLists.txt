cmake_minimum_required(VERSION 3.2)

project( rubix-solver )

find_package( OpenCV REQUIRED )
add_executable (rubixsolver "rubixsolver.cpp" "rubixsolver.h")


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable( rubix-solver source/main.cpp )
target_link_libraries( rubix-solver ${OpenCV_LIBS} )