cmake_minimum_required(VERSION 3.21)
project(sketchFiller)
set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "T:/bin/opencv/build")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(processing)
add_subdirectory(showroom)



