//
// Created by thahnen on 24.06.19.
//

#pragma once
#ifndef AIOLOS_MATRIXFUNCTIONS_H
#define AIOLOS_MATRIXFUNCTIONS_H

#include <limits>
#include <opencv2/opencv.hpp>


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


#endif //AIOLOS_MATRIXFUNCTIONS_H
