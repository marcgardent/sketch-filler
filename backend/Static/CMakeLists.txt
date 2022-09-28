project(BackendStatic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Gui Qml Quick OpenGL)

qt_add_library(BackendStatic STATIC ${BACKEND_SOURCES})
set_target_properties(
        BackendStatic PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY BackendStatic
)

qt_add_qml_module(BackendStatic
        URI backend
        VERSION 1.0
        SOURCES ${BACKEND_SOURCES}
        )

target_link_libraries(BackendStatic PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        ${OpenCV_LIBS}
        ${BACKEND_LIBS}
        )

target_include_directories(BackendStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
