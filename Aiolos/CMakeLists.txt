cmake_minimum_required(VERSION 3.14)
project(Aiolos
        VERSION 2.0.5
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

#target_compile_definitions(${PROJECT_NAME} PUBLIC AIOLOS_FEATURE_NO_ASSERT)


# After library is build, find it!
find_library(Aiolos REQUIRED)


########################################################################################################################
#       BUILD OPTIONS FOR RUNNING THE TEST PROGRAM
########################################################################################################################
add_executable(Aiolos_Test
        include/util/VisualizationHelper.h
        main.cpp)

target_link_libraries(Aiolos_Test
        PUBLIC
            Aiolos
            OpenMP::OpenMP_CXX
            ${OpenCV_LIBS})


########################################################################################################################
#       BUILD OPTIONS FOR RUNNING THE TEST:
#
#           - target name:      Aiolos_test_keyframes_main_angle
#           - file:             keyframes.main_angle.cpp
#           - input type:       image (png), keyframes from video c.1W.avi
#           - function:         main_angle
#           - implementation:   STANDARD + SCHEME2
#           - methods:          -/-
########################################################################################################################
add_executable(Aiolos_test_keyframes_main_angle
        tests/keyframes.main_angle.cpp)

target_link_libraries(Aiolos_test_keyframes_main_angle
        PUBLIC
            Aiolos)


########################################################################################################################
#       BUILD OPTIONS FOR RUNNING THE TEST:
#
#           - target name:      Aiolos_test_keyframes_main_angles
#           - file:             keyframes.main_angles.cpp
#           - input type:       image (png), keyframes from video c.1W.avi
#           - function:         main_angles
#           - implementation:   STANDARD + SCHEME2
#           - methods:          SPLIT_IMAGE_2x1 + SPLIT_IMAGE_3x1 + SPLIT_IMAGE_4x1
########################################################################################################################