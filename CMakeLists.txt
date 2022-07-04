cmake_minimum_required(VERSION 3.5)

project(PointCloudViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(PCL CONFIG REQUIRED)
find_package(VTK REQUIRED)

find_path(LIBLAS_INCLUDE_DIR liblas/liblas.hpp)
find_library(libLAS_LIBRARIES liblas)
include_directories(${LIBLAS_INCLUDE_DIR})

enable_language("RC")

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.rc
)

add_executable(PointCloudViewer
    ${PROJECT_SOURCES}
)

target_link_libraries(PointCloudViewer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${VTK_LIBRARIES}
    ${PCL_LIBRARIES}
    ${libLAS_LIBRARIES}
)

set_target_properties(PointCloudViewer PROPERTIES
    WIN32_EXECUTABLE TRUE
)

