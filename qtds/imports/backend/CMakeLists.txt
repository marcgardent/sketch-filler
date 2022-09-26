qt_add_library(backend STATIC)

qt_add_qml_module(backend
        URI backend
        VERSION 1.0
        SOURCES renderer.cpp renderer.h
        )

target_link_libraries(backend PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        ${OpenCV_LIBS}
        )