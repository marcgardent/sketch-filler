project(BackendShared LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Gui Qml Quick OpenGL)

qt_add_library(BackendShared SHARED ${BACKEND_SOURCES})

qt_add_qml_module(BackendShared
        URI backend
        VERSION 1.0
        SOURCES ${BACKEND_SOURCES}
        )

target_link_libraries(BackendShared PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        ${OpenCV_LIBS}
        )

target_include_directories(BackendShared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
