cmake_minimum_required(VERSION 3.14)
project(Aiolos)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(Aiolos main.cpp)
target_link_libraries(Aiolos ${OpenCV_LIBS})
