project(Polygons)
add_library(Polygons polygons.cpp simplify.cpp)
target_link_libraries(Polygons INTERFACE ${OpenCV_LIBS}  Points)

target_include_directories(Polygons PUBLIC "${PROJECT_SOURCE_DIR}/processing/points")
