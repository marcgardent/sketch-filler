project(Contrib)
add_library(Contrib thinning.cpp)
target_link_libraries( Contrib INTERFACE ${OpenCV_LIBS})