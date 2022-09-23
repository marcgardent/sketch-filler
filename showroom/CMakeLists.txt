project (showroom)
add_executable(showroom main.cpp)
target_link_libraries( showroom PRIVATE ${OpenCV_LIBS} Contrib Pixels Polygons)
