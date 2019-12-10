cmake_minimum_required(VERSION 3.14)
project(Aiolos
        VERSION         2.4.0
        DESCRIPTION     "GLCM-Based Texture Orientation Estimation Method"
        HOMEPAGE_URL    "https://github.com/thahnen/Aiolos"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 14)

# OpenMP required for parallized working (astonishing speed increase!)
find_package(OpenMP REQUIRED)

# OpenCV required for image processing
if (WIN32)
	# By now OpenCV has to be installed here for CMake to find it!
	# TODO: Fix this later!
    set(OpenCV_DIR C:/opencv/build/x64/vc15/lib)
endif(WIN32)
find_package(OpenCV REQUIRED)

include_directories(./include)


########################################################################################################################
#       BUILD OPTIONS FOR CREATING A SHARED LIBRARY:
#           1. No features
#           2. Feature: more types
#           3. Feature: sub images
#           4. Feature: more types + sub images
#
#       REVIEW: Exit code 127 is normal (because no "program" is run after building)
########################################################################################################################
# No features
add_library(${PROJECT_NAME}
        SHARED
            include/util/VectorFunctions.h
            include/util/MatrixFunctions.h
            src/util/MatrixFunctions.cpp
            include/impl/Distribution.h
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


# Feature: more types
add_library(Aiolos.MT
        SHARED
            include/util/VectorFunctions.h
            include/util/MatrixFunctions.h
            src/util/MatrixFunctions.cpp
            include/impl/Distribution.h
            include/impl/Scheme1.h
            include/impl/Scheme2.h
            include/impl/Scheme3.h
            include/impl/Standard.h
            src/GLCM.cpp)

set_target_properties(Aiolos.MT
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/GLCM.h)

target_include_directories(Aiolos.MT
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(Aiolos.MT
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS})

target_compile_definitions(Aiolos.MT
        PUBLIC
            AIOLOS_FEATURE_MORE_TYPE_SUPPORT)


# Feature: sub images
add_library(Aiolos.SI
        SHARED
            include/util/VectorFunctions.h
            include/util/MatrixFunctions.h
            src/util/MatrixFunctions.cpp
            include/impl/Distribution.h
            include/impl/Scheme1.h
            include/impl/Scheme2.h
            include/impl/Scheme3.h
            include/impl/Standard.h
            src/GLCM.cpp)

set_target_properties(Aiolos.SI
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/GLCM.h)

target_include_directories(Aiolos.SI
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(Aiolos.SI
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS})

target_compile_definitions(Aiolos.SI
        PUBLIC
            AIOLOS_FEATURE_SUB_IMAGE)


# Feature: more types + sub images
add_library(Aiolos.MT.SI
        SHARED
            include/util/VectorFunctions.h
            include/util/MatrixFunctions.h
            src/util/MatrixFunctions.cpp
            include/impl/Distribution.h
            include/impl/Scheme1.h
            include/impl/Scheme2.h
            include/impl/Scheme3.h
            include/impl/Standard.h
            src/GLCM.cpp)

set_target_properties(Aiolos.MT.SI
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/GLCM.h)

target_include_directories(Aiolos.MT.SI
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(Aiolos.MT.SI
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS})

target_compile_definitions(Aiolos.MT.SI
        PUBLIC
            AIOLOS_FEATURE_MORE_TYPE_SUPPORT
            AIOLOS_FEATURE_SUB_IMAGE)


########################################################################################################################
#       AFTER LIBRARIES ARE BUILD FIND THEM!
########################################################################################################################
find_library(Aiolos REQUIRED)
find_library(Aiolos.MT REQUIRED)
find_library(Aiolos.SI REQUIRED)
find_library(Aiolos.MT.SI Required)


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
#       BUILD OPTIONS FOR THE BENCHMARKS:
#		TODO: does not complete on Windows due to errors inside the OpenCV-implementation!
#
#           - target name:      Aiolos_benchmark_main_angle
#           - file:             benchmark.main_angle.cpp
#           - input type:       test images
#           - function:         main_angle
#           - implementation:   STANDARD + SCHEME2
#           - methods:          -/-
########################################################################################################################
add_executable(Aiolos_benchmark_main_angle
        tests/benchmark.main_angle.cpp)

target_link_libraries(Aiolos_benchmark_main_angle
        PUBLIC
            Aiolos)



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
        include/util/VisualizationHelper.h
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