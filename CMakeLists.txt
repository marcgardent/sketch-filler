cmake_minimum_required(VERSION 3.21)
project(sketchFiller)
set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "T:/bin/opencv/build")
set(CMAKE_PREFIX_PATH "T:/ProgramData/Qt6/6.3.2/msvc2019_64")

find_package(OpenCV REQUIRED)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/points
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/pixels
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/polygons
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/contrib
        ${CMAKE_CURRENT_SOURCE_DIR}/processing/morphologicals
)

include_directories(
        "T:/ProgramData/Qt6/6.3.2/msvc2019_64/include"
        "T:/ProgramData/Qt6/6.3.2/msvc2019_64/include/QtOpenGL"
        "T:/ProgramData/Qt6/6.3.2/msvc2019_64/include/QtOpenGLWidgets"
)
add_subdirectory(processing)
add_subdirectory(showroom)
add_subdirectory(qtds)
