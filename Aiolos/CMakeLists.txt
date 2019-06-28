cmake_minimum_required(VERSION 3.14)
project(Aiolos)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(./include)
file(GLOB HEADERS "include/*/*.h" "include/*.h")
file(GLOB SOURCES "src/*/*.cpp" "src/*.cpp")

add_executable(Aiolos ${HEADERS} ${SOURCES} main.cpp include/BasicFunctions.h include/GLCM.h include/Scheme1.h include/Scheme2.h include/Scheme3.h)
target_link_libraries(Aiolos OpenMP::OpenMP_CXX)
target_link_libraries(Aiolos ${OpenCV_LIBS})
