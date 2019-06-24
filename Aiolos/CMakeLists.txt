cmake_minimum_required(VERSION 3.14)
project(Aiolos)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

find_package(OpenCV REQUIRED)
if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

include_directories(./include)
file(GLOB HEADERS "include/*/*.h" "include/*.h")
file(GLOB SOURCES "src/*/*.cpp" "src/*.cpp")

add_executable(Aiolos ${HEADERS} ${SOURCES} main.cpp include/BasicFunctions.h include/GLCM.h)
target_link_libraries(Aiolos OpenMP::OpenMP_CXX)
target_link_libraries(Aiolos ${OpenCV_LIBS})
