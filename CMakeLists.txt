cmake_minimum_required(VERSION 3.21)

project(qmlgstdemo VERSION 1.0.0 LANGUAGES CXX)

# C++ Options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Export compile commands for IDE that can integrate with them.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH  ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Setup CMake prefix path to find GStreamer
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/gstreamer/gstreamer-install)
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# Debugging for find_package(...)
# set(CMAKE_FIND_DEBUG_MODE TRUE)

# Bring in dependencies Qt6 + GStreamer
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickWidgets)
qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(GObject REQUIRED)
find_package(GStreamer REQUIRED)

# Add "good" executable
add_subdirectory(src/good)
add_subdirectory(src/bad)