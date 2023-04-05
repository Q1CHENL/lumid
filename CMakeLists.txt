cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
project(lumid)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(/usr/include/qt5/QxtWidgets)
include_directories(/usr/include/qt5/QxtCore)

find_library(QTX_CORE_LIB QxtCore-qt5 HINTS /usr/lib64/)
find_library(QTX_WIDGETS_LIB QxtWidgets-qt5 HINTS /usr/lib64/)

add_executable(lumid Main.cpp MainWindow.cpp MainWindow.hpp Wrappers.cpp Wrappers.hpp)
target_link_libraries(lumid
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${QTX_CORE_LIB}
        ${QTX_WIDGETS_LIB}
        )

