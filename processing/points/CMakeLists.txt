project(Points)
add_library(Points INTERFACE)
target_link_libraries( Points INTERFACE ${OpenCV_LIBS})
