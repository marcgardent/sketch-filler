cmake_minimum_required(VERSION 3.18)
project(ColorizeApp LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Gui Qml Quick OpenGL)
find_package(Backend  PATHS ${BACKEND_LIBRARY_PATH})
qt_add_executable(ColorizeApp
        src/main.cpp
)

qt_add_resources(ColorizeApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(ColorizeApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    ${OpenCV_LIBS}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)


#target_link_libraries(ColorizeApp PRIVATE BackendStatic BackendStaticplugin )
