project(Pixels)
add_library(Pixels thin.cpp)
target_link_libraries( Pixels INTERFACE ${OpenCV_LIBS})