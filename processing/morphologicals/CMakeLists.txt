project(Morphologicals)
add_library(Morphologicals morphologicals.cpp)
target_link_libraries( Morphologicals INTERFACE ${OpenCV_LIBS})