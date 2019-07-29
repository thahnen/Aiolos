cmake_minimum_required(VERSION 3.14)
project(Aiolos)

set(CMAKE_CXX_STANDARD 17)

# OpenMP required for parallized working (astonishing speed increase!)
find_package(OpenMP REQUIRED)
# OpenCV required for the rest
find_package(OpenCV REQUIRED)

include_directories(./include)
add_executable(Aiolos main.cpp include/util/BasicFunctions.h include/GLCM.h include/impl/Scheme1.h include/impl/Scheme2.h include/impl/Scheme3.h include/impl/Standard.h include/util/VisualizationHelper.h)

target_link_libraries(Aiolos OpenMP::OpenMP_CXX ${OpenCV_LIBS})
