cmake_minimum_required(VERSION 3.14)
project(Aiolos)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(./include)

add_executable(Aiolos main.cpp include/BasicFunctions.h include/GLCM.h include/implementations/Scheme1.h include/implementations/Scheme2.h include/implementations/Scheme3.h include/implementations/Standard.h)
target_link_libraries(Aiolos OpenMP::OpenMP_CXX)
target_link_libraries(Aiolos ${OpenCV_LIBS})
