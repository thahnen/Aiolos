//
// Created by thahnen on 30.07.19.
//


#define MACRO(x) #x

#include "util/VectorFunctions.h"
#include "util/MatrixFunctions.h"
#include "impl/Distribution.h"
#include "GLCM.h"


/***********************************************************************************************************************
 *
 *      Functions calling other functions
 *
 ***********************************************************************************************************************/

/// Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
unsigned int GLCM::main_angle(const cv::Mat& image, Implementation impl, unsigned int max_r) {
    return main_angle(image, impl, GLCM::Range(0, 179), max_r);
}


/// Calculates the dominant texture orientations of the image (one or more + duplicates possible).
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, Implementation impl, Method meth, unsigned int max_r) {
    return main_angles(image, impl, meth, GLCM::Range(0, 179), max_r);
}


/// Calculates the dominant texture orientations of the image (one or more possible).
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat &image, GLCM::Implementation impl, GLCM::Method meth,
                                                unsigned int max_r) {
    return main_angles_set(image, impl, meth, GLCM::Range(0, 179), max_r);
}



/***********************************************************************************************************************
 *
 *      Actual implementation of the functions
 *
 ***********************************************************************************************************************/

/// Calculates the one dominant texture orientation of an image for specific angles.
unsigned int GLCM::main_angle(const cv::Mat& image, Implementation impl, const Range& range, unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<double> orientation_distribution = getAngleDistribution(image, impl, max_radius, range);

    return range.first + std::distance(
            orientation_distribution.begin(),
            min_element(orientation_distribution.begin(), orientation_distribution.end())
    );
}


/// Calculates the dominant texture orientations of the image (one or more + duplicates possible) for specific angles.
// TODO: Noch nicht eingefügte Möglichkeiten bedenken!
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, Implementation impl, Method meth, const Range& range,
                                                unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<unsigned int> angles;

    // Test for image splitting method!
    if (meth == SPLIT_IMAGE_2x2 || meth == SPLIT_IMAGE_3x3 || meth == SPLIT_IMAGE_4x4
        || meth == SPLIT_IMAGE_1x2 || meth == SPLIT_IMAGE_1x3 || meth == SPLIT_IMAGE_1x4
        || meth == SPLIT_IMAGE_2x1 || meth == SPLIT_IMAGE_3x1 || meth == SPLIT_IMAGE_4x1) {
        Util::split_image(image, angles, impl, meth, range, max_radius);
        return angles;
    }

    std::vector<double> orientation_dist = getAngleDistribution(image, impl, max_radius, range);
    auto begin = static_cast<unsigned int>(range.first);

    switch (meth) {
        case TOP_2:
            angles = Util::getLowestIndizes(orientation_dist, 2);
            break;
        case TOP_3:
            angles = Util::getLowestIndizes(orientation_dist, 3);
            break;
        default:
            double value;

            if (meth == MEDIAN) {
                value = Util::getMedianValue(orientation_dist);
            } else if (meth == AVERAGE) {
                value = Util::getAverageValue(orientation_dist);
            } else if (meth == L_QUARTILE) {
                value = Util::getQuantileValue(orientation_dist, LOWER_QUARTILE);
            } else {
                throw std::runtime_error("[GLCM::main_angles] Other Methods not implemented yet!");
            }

            // TODO: Better parallelization -> see: https://stackoverflow.com/a/18671256
            //#pragma omp declare reduction(vector_push_back : \
            //                                std::vector<unsigned int> : \
            //                                omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

#ifndef WIN32
            #pragma omp parallel for //ordered //reduction(vector_push_back: angles)
#endif
            for (auto it = orientation_dist.begin(); it < orientation_dist.end(); ++it) {
                if (*it >= value) continue;
                angles.push_back(begin + std::distance(orientation_dist.begin(), it));
            }

            return angles;
    }

    // TODO: Better parallelization -> see: https://stackoverflow.com/a/43169193
    //#pragma omp declare reduction(vector_unsigned_int_plus : \
    //                                std::vector<unsigned int> : std::transform( \
    //                                    omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), \
    //                                    std::plus<unsigned int>() \
    //                                )) initializer(omp_priv = decltype(omp_orig)(omp_orig.size()))

#ifndef WIN32
    #pragma omp parallel for //reduction(vector_unsigned_int_plus: angles)
#endif
    for (int i = 0; i < angles.size(); i++) {
        angles[i] += begin;
    }

    return angles;
}


/// Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat &image, GLCM::Implementation impl, GLCM::Method meth,
                                                const GLCM::Range &range, unsigned int max_r) {
    std::vector<unsigned int> angles = main_angles(image, impl, meth, range, max_r);
    return std::set<unsigned int>(angles.begin(), angles.end());
}



#ifdef AIOLOS_FEATURE_MORE_TYPE_SUPPORT
/***********************************************************************************************************************
 *
 *      Functions calling other functions (with other type support)
 *
 ***********************************************************************************************************************/

/// Calculates the one dominant texture orientation of an image for specific angles.
unsigned int GLCM::main_angle(const cv::Mat& image, GLCM::Implementation impl, const cv::Range& range, unsigned int max_r) {
    return main_angle(image, impl, GLCM::Range(range.start, range.end), max_r);
}


/// Calculates the dominant texture orientations of the image (one or more + duplicates possible) for specific angles.
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const cv::Range& range, unsigned int max_r) {
    return main_angles(image, impl, meth, GLCM::Range(range.start, range.end), max_r);
}


/// Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const cv::Range& range, unsigned int max_r) {
    return main_angles_set(image, impl, meth, GLCM::Range(range.start, range.end), max_r);
}
#endif



#ifdef AIOLOS_FEATURE_SUB_IMAGE
/***********************************************************************************************************************
 *
 *      Sub-image functions calling other functions
 *
 ***********************************************************************************************************************/

/// Calculates the dominant texture orientation of a sub-image (equals the "min_theta"-function from the paper).
unsigned int GLCM::main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl, unsigned int max_r) {
    return main_angle(image(boundaries), impl, max_r);
}


/// Calculates the one dominant texture orientation of a sub-image for specific angles.
unsigned int GLCM::main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                const GLCM::Range& range, unsigned int max_r) {
    return main_angle(image(boundaries), impl, range, max_r);
}



/// Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible).
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, unsigned int max_r) {
    return main_angles(image(boundaries), impl, meth, max_r);
}


/// Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible) for specific angles.
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const GLCM::Range& range, unsigned int max_r) {
    return main_angles(image(boundaries), impl, meth, range, max_r);
}


/// Calculates the dominant texture orientations of the sub-image (one or more possible).
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, unsigned int max_r) {
    return main_angles_set(image(boundaries), impl, meth, max_r);
}


/// Calculates the dominant texture orientations of the sub-image (one or more possible) for specific angles.
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const GLCM::Range& range, unsigned int max_r) {
    return main_angles_set(image(boundaries), impl, meth, range, max_r);
}


#ifdef AIOLOS_FEATURE_MORE_TYPE_SUPPORT
/// Calculates the one dominant texture orientation of a sub-image for specific angles.
unsigned int GLCM::main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                const cv::Range& range, unsigned int max_r) {
    return main_angle(image(boundaries), impl, GLCM::Range(range.start, range.end), max_r);
}


/// Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible) for specific angles.
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const cv::Range& range, unsigned int max_r) {
    return main_angles(image(boundaries), impl, meth, GLCM::Range(range.start, range.end), max_r);
}


/// Calculates the dominant texture orientations of the sub-image (one or more possible) for specific angles.
std::set<unsigned int> GLCM::main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const cv::Range& range, unsigned int max_r) {
    return main_angles_set(image(boundaries), impl, meth, GLCM::Range(range.start, range.end), max_r);
}
#endif
#endif



/***********************************************************************************************************************
 *
 *      Implementation of support functions
 *
 ***********************************************************************************************************************/

/// Which features activated in library (support for multiple versions)
GLCM::FEATURES GLCM::getFeatures() {
    GLCM::FEATURES features{};
    std::pair<std::string, std::string> f{};

#ifdef AIOLOS_FEATURE_MORE_TYPE_SUPPORT
    f.first = MACRO(AIOLOS_FEATURE_MORE_TYPE_SUPPORT);
    f.second = "Enables support for more types, just for more convenience!";
    features.emplace_back(f);
#endif

#ifdef AIOLOS_FEATURE_SUB_IMAGE
    f.first = MACRO(AIOLOS_FEATURE_SUB_IMAGE);
    f.second = "Enables support for subimages, easier functions but library is bigger!";
    features.emplace_back(f);
#endif

#ifdef AIOLOS_FEATURE_NO_ASSERT
    f.first = MACRO(AIOLOS_FEATURE_NO_ASSERT);
    f.second = "Disables all assertions using 'assert'!";
    features.emplace_back(f);
#endif

#ifdef AIOLOS_TEST_SCHEME2_GLCM
    f.first = MACRO(AIOLOS_TEST_SCHEME2_GLCM);
    f.second = "Uses a slightly alternative calculation inside the (private) SCHEME2::GLCM function!";
    features.emplace_back(f);
#endif

    return features;
}


/// Which debug symbols are set in library (for debugging)
GLCM::DEBUGS GLCM::getDebugs() {
    GLCM::DEBUGS debugs{};
    std::pair<std::string, std::string> d{};

#ifdef AIOLOS_DEBUG_ANGLE_DISTRIBUTION
    d.first = MACRO(AIOLOS_DEBUG_ANGLE_DISTRIBUTION);
    d.second = "Prints out the calculated value for every angle to debug the (private) GLCM::getAngleDistribution function";
    debugs.emplace_back(d);
#endif

#ifdef AIOLOS_DEBUG_SCHEME2_GLCM
    d.first = MACRO(AIOLOS_DEBUG_SCHEME2_GLCM);
    d.second = "Prints out the calculated G-values in comparison to the GLCM-Matrix-Size to debug the (private) SCHEME2::GLCM function";
    debugs.emplace_back(d);
#endif

#ifdef AIOLOS_TEST_SCHEME2_GLCM
    d.first = MACRO(AIOLOS_TEST_SCHEME2_GLCM);
    d.second = "Test a different implementation for the c-values in the (private) SCHEME2::GLCM function";
    debugs.emplace_back(d);
#endif

    return debugs;
}