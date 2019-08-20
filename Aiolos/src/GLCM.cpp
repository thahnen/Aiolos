//
// Created by thahnen on 30.07.19.
//

#include <numeric>

#include "util/VectorFunctions.h"
#include "util/MatrixFunctions.h"
#include "impl/Distribution.h"
#include "GLCM.h"


/**
 *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
 *  TODO: ggf inline vorschlagen?
 */
unsigned int GLCM::main_angle(const cv::Mat& image, Implementation impl, unsigned int max_r) {
    return main_angle_range(image, impl, Range(0, 179), max_r);
}


/**
 *  Calculates the one dominant texture orientation of an image for specific angles.
 */
unsigned int GLCM::main_angle_range(const cv::Mat& image, Implementation impl, const Range& range, unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<double> orientation_distribution = getAngleDistribution(image, impl, max_radius, range);

    return range.first + std::distance(
            orientation_distribution.begin(),
            min_element(orientation_distribution.begin(), orientation_distribution.end())
    );
}


/**
 *  Calculates the dominant texture orientations of the image (one or more possible).
 *  TODO: ggf inline vorschlagen?
 */
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, Implementation impl, Method meth, unsigned int max_r) {
    return main_angles_range(image, impl, meth, Range(0, 179), max_r);
}


/**
 *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
 *  TODO: Zusammenfassen GLCM::main_angles_range + GLCM::CT::main_angles_range_
 *  TODO: Noch nicht eingefügte Möglichkeiten bedenken!
 *  TODO: Umstellen, da "orientation_dist" nicht in allen Fällen benötigt wird!
 */
std::vector<unsigned int> GLCM::main_angles_range(const cv::Mat& image, Implementation impl, Method meth,
                                                    const Range& range, unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<unsigned int> angles;

    if (meth == SPLIT_IMAGE_4) {
        Util::split_image(image, angles, impl, range, max_radius);
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
                throw std::runtime_error("[GLCM::main_angles_range] Other Options not implemented yet!");
            }

            // TODO: Better parallelization -> see: https://stackoverflow.com/a/18671256
            //#pragma omp declare reduction(vector_push_back : \
            //                                std::vector<unsigned int> : \
            //                                omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

            #pragma omp parallel for ordered //reduction(vector_push_back: angles)
            for (auto it = orientation_dist.begin(); it < orientation_dist.end(); ++it) {
                if (*it < value) {
                    angles.push_back(begin + std::distance(orientation_dist.begin(), it));
                }
            }

            return angles;
    }

    // TODO: Better parallelization -> see: https://stackoverflow.com/a/43169193
    //#pragma omp declare reduction(vector_iterator_add : \
    //                                std::vector<unsigned int> : \
    //                                std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), \
    //                                                omp_out.begin(), std::plus<unsigned int>())) \
    //                                initializer(omp_priv = omp_orig)

    #pragma omp parallel for ordered //reduction(vector_iterator_add: angles)
    for (auto it = angles.begin(); it < angles.end(); ++it) {
        *it += begin;
    }

    return angles;
}


/**
 *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
 *  TODO: ggf inline vorschlagen?
 */
unsigned int GLCM::CT::main_angle(const cv::Mat& image, Implementation impl, unsigned int max_r) {
    return main_angle_range_(image, impl, Range(0, 179), max_r);
}


/**
 *  Calculates the one dominant texture orientation of an image for specific angles.
 */
unsigned int GLCM::CT::main_angle_range_(const cv::Mat& image, Implementation impl, const Range& range,
                                            unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<double> orientation_distribution = getAngleDistribution_(image, impl, max_radius, range);

    return range.first + std::distance(
            orientation_distribution.begin(),
            min_element(orientation_distribution.begin(), orientation_distribution.end())
    );
}


/**
 *  Calculates the dominant texture orientations of the image (one or more possible).
 *  TODO: ggf inline vorschlagen?
 */
std::vector<unsigned int> GLCM::CT::main_angles(const cv::Mat& image, Implementation impl, Method meth,
                                                    unsigned int max_r) {
    return main_angles_range_(image, impl, meth, Range(0, 179), max_r);
}


/**
 *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
 *  TODO: Zusammenfassen GLCM::main_angles_range + GLCM::CT::main_angles_range_
 *  TODO: Noch nicht eingefügte Möglichkeiten bedenken!
 *  TODO: Umstellen, da "orientation_dist" nicht in allen Fällen benötigt wird!
 */
std::vector<unsigned int> GLCM::CT::main_angles_range_(const cv::Mat& image, Implementation impl, Method meth,
                                                        const Range& range, unsigned int max_r) {
    unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
    std::vector<unsigned int> angles;

    if (meth == SPLIT_IMAGE_4) {
        Util::split_image(image, angles, impl, range, max_radius, false);
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
                throw std::runtime_error("[GLCM::main_angles_range] Other Options not implemented yet!");
            }

            // TODO: Better parallelization -> see: https://stackoverflow.com/a/18671256
            //#pragma omp declare reduction(vector_push_back : \
            //                                std::vector<unsigned int> : \
            //                                omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

            #pragma omp parallel for ordered //reduction(vector_push_back: angles)
            for (auto it = orientation_dist.begin(); it < orientation_dist.end(); ++it) {
                if (*it < value) {
                    angles.push_back(begin + std::distance(orientation_dist.begin(), it));
                }
            }

            return angles;
    }

    // TODO: Better parallelization -> see: https://stackoverflow.com/a/43169193
    //#pragma omp declare reduction(vector_iterator_add : \
    //                                std::vector<unsigned int> : \
    //                                std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), \
    //                                                omp_out.begin(), std::plus<unsigned int>())) \
    //                                initializer(omp_priv = omp_orig)

    #pragma omp parallel for ordered //reduction(vector_iterator_add: angles)
    for (auto it = angles.begin(); it < angles.end(); ++it) {
        *it += begin;
    }

    return angles;
}