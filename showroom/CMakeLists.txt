project (showroom)
add_executable(showroom debug.cpp)
target_link_libraries( showroom PRIVATE ${OpenCV_LIBS} Contrib Pixels Polygons Morphologicals)


project (hotspot)
add_executable(hotspot main.cpp)
target_link_libraries( hotspot PRIVATE ${OpenCV_LIBS} Contrib Pixels Polygons Morphologicals)
