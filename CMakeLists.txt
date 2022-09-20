cmake_minimum_required(VERSION 3.21)
project (sketch_filler)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED PATHS "T:/bin/opencv")

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(sketch_filler main.cpp)
target_link_libraries( sketch_filler ${OpenCV_LIBS} )
