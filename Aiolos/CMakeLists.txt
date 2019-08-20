cmake_minimum_required(VERSION 3.14)
project(Aiolos
        VERSION 1.7.0
        DESCRIPTION "GLCM-Based Texture Orientation Estimation Method"
        HOMEPAGE_URL "https://github.com/thahnen/Aiolos"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)           # OpenMP required for parallized working (astonishing speed increase!)
find_package(OpenCV REQUIRED)           # OpenCV required for the rest

include_directories(./include)


########################################################################################################################
#       BUILD OPTIONS FOR CREATING A SHARED LIBRARY
#       => Use "target_compile_definitions" to specify different build options!
#
#       REVIEW: Exit code 127 is normal (because no "program" is run after building)
########################################################################################################################
add_library(${PROJECT_NAME}
        SHARED
            include/util/VectorFunctions.h
            #src/util/VectorFunctions.cpp
            include/util/MatrixFunctions.h
            src/util/MatrixFunctions.cpp
            include/impl/Distribution.h
            #src/impl/Distribution.cpp
            include/impl/Scheme1.h
            include/impl/Scheme2.h
            include/impl/Scheme3.h
            include/impl/Standard.h
            src/GLCM.cpp)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/GLCM.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS})

#target_compile_definitions(${PROJECT_NAME} ...)


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
