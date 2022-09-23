cmake_minimum_required(VERSION 3.21)
project(sketchFiller)
set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "T:/bin/opencv/build")

find_package(OpenCV REQUIRED)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/points
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/pixels
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/polygons
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/contrib
)


add_subdirectory(processing)
add_subdirectory(showroom)



