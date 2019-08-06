cmake_minimum_required(VERSION 3.14)
project(Aiolos
        VERSION 1.0.0
        DESCRIPTION "GLCM-Based Texture Orientation Estimation Method"
        HOMEPAGE_URL "https://github.com/thahnen/Aiolos"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)           # OpenMP required for parallized working (astonishing speed increase!)
find_package(OpenCV REQUIRED)           # OpenCV required for the rest

include_directories(./include)


########################################################################################################################
#       BUILD OPTIONS FOR CREATING A SHARED LIBRARY
#       TODO: Runs into exit code 127 => no Problem!
########################################################################################################################
add_library(${PROJECT_NAME}
        SHARED
            include/util/VectorMath.h
            include/util/MatrixFunctions.h
            include/impl/Distribution.h
            include/impl/Scheme1.h
            include/impl/Scheme2.h
            include/impl/Scheme3.h
            include/impl/Standard.h
            src/GLCM.cpp)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
            CXX_VISIBILITY_PRESET hidden
            VERSION ${PROJECT_VERSION}
            PUBLIC_HEADER include/GLCM.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS})


########################################################################################################################
#       BUILD OPTIONS FOR RUNNING THE TEST PROGRAM
########################################################################################################################
find_library(Aiolos REQUIRED)

add_executable(Aiolos_Test
        include/util/VisualizationHelper.h
        main.cpp)

target_link_libraries(Aiolos_Test
        PUBLIC
            Aiolos
            OpenMP::OpenMP_CXX
            ${OpenCV_LIBS})
