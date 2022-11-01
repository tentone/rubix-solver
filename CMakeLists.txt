cmake_minimum_required(VERSION 3.2)

project( rubix-solver )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable( rubix-solver ./main.cpp  )
target_link_libraries( rubix-solver ${OpenCV_LIBS})