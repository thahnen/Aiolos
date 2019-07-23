//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_BASICFUNCTIONS_H
#define AIOLOS_BASICFUNCTIONS_H

#include <opencv2/opencv.hpp>


/**
 *  Returns the maximum gray value of the image
 *
 *  @param image        the given image
 *  @return             the value
 *
 *  REVIEW: Usage does not depend on specific Mat-Type
 */
unsigned int max_gray_value(const cv::Mat& image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // signed char      -> 2^7 highest value
            return 128;
        case CV_8U:
            // unsigned char    -> 2^8 highest value
            return 256;
        case CV_16S:
            // signed short     -> 2^15 highest value
            return 32768;
        case CV_16U:
            // unsigned short   -> 2^16 highest value
            return 65536;
        case CV_32S:
            // signed int       -> 2^31 highest value
            return 2147483648;
        default:
            throw std::runtime_error("[max_gray_value] Unsupported Mat-type!");
    }
}


#endif //AIOLOS_BASICFUNCTIONS_H
