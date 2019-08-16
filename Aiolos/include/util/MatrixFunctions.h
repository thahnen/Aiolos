//
// Created by thahnen on 24.06.19.
//

#pragma once
#ifndef AIOLOS_MATRIXFUNCTIONS_H
#define AIOLOS_MATRIXFUNCTIONS_H

#include <limits>
#include <opencv2/opencv.hpp>
#include "GLCM.h"


/**
 *  Returns the maximum gray value of the image-matrix
 *
 *  @param image        the given image
 *  @return             the value
 *
 *  REVIEW: Usage does not depend on specific Mat-Type
 *  REVIEW: Assert correct maximum values using standard library!
 */
int max_gray_value(const cv::Mat& image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // (signed) char    -> 2^7 highest value
            return static_cast<int>(std::numeric_limits<char>::max());
        case CV_8U:
            // unsigned char    -> 2^8 highest value
            return static_cast<int>(std::numeric_limits<unsigned char>::max());
        case CV_16S:
            // (signed) short   -> 2^15 highest value
            return static_cast<int>(std::numeric_limits<short>::max());
        case CV_16U:
            // unsigned short   -> 2^16 highest value
            return static_cast<int>(std::numeric_limits<unsigned short>::max());
        case CV_32S:
            // (signed) int     -> 2^31 highest value
            return std::numeric_limits<int>::max();
        default:
            throw std::runtime_error("[max_gray_value] Unsupported Mat-type!");
    }
}


/**
 *  Splits image in 4 subimages to evaluate their angles
 *
 *  @param image        the given image
 *  @param angles       the returned vector of found angles
 *  @param impl         which implementation of the GLCM shall be used
 *  @param range        interval of angles to consider!
 *  @param max_r        maximum radius
 *  @param rt           if function uses runtime implementation or not (runtime is default)
 *
 *  TODO: Parallelize for loop!
 */
void split_image(const cv::Mat& image, std::vector<unsigned int>& angles, GLCM::Implementation impl,
                    const GLCM::Range& range, unsigned int max_r, bool rt = true) {
    int middle_width = ceil(image.cols/2);
    int middle_height = ceil(image.rows/2);

    std::set<unsigned int> values;

    cv::Mat m1 = image(cv::Range(0, middle_height), cv::Range(0, middle_width));
    cv::Mat m2 = image(cv::Range(0, middle_height), cv::Range(middle_width + 1, image.cols));
    cv::Mat m3 = image(cv::Range(middle_height + 1, image.rows), cv::Range(0, middle_width));
    cv::Mat m4 = image(cv::Range(middle_height + 1, image.rows), cv::Range(middle_width + 1, image.cols));

    if (rt) {
        values.insert(GLCM::main_angle_range(m1, impl, range, max_r));
        values.insert(GLCM::main_angle_range(m2, impl, range, max_r));
        values.insert(GLCM::main_angle_range(m3, impl, range, max_r));
        values.insert(GLCM::main_angle_range(m4, impl, range, max_r));
    } else {
        values.insert(GLCM::CT::main_angle_range_(m1, impl, range, max_r));
        values.insert(GLCM::CT::main_angle_range_(m2, impl, range, max_r));
        values.insert(GLCM::CT::main_angle_range_(m3, impl, range, max_r));
        values.insert(GLCM::CT::main_angle_range_(m4, impl, range, max_r));
    }

    /*#pragma omp parallel for
    for (std::set<unsigned int>::const_iterator it = values.begin(); it < values.end(); it++) {
        angles.push_back(*it);
    }*/

    for (unsigned int value : values) {
        angles.push_back(value);
    }
}


#endif //AIOLOS_MATRIXFUNCTIONS_H