cmake_minimum_required(VERSION 3.18)
project(ColorizeApp LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/backend ${CMAKE_CURRENT_BINARY_DIR}/backend CACHE STRING "" FORCE)

find_package(Qt6 COMPONENTS Gui Qml Quick OpenGL)

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

add_subdirectory(imports/backend)
target_link_libraries(ColorizeApp PRIVATE
        backendplugin
        )
