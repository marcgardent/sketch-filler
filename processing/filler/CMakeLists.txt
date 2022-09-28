project(Filler)
add_library(Filler Filler.cpp)
target_link_libraries( Filler INTERFACE ${OpenCV_LIBS})