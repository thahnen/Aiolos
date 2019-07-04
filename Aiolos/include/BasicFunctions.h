//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_BASICFUNCTIONS_H
#define AIOLOS_BASICFUNCTIONS_H

#include <opencv4/opencv2/opencv.hpp>


/**
 *  Returns the maximum gray value of the image
 *
 *  @param image        the given image
 *  @return             the value, based on the matrix-type
 */
unsigned int max_gray_value(const cv::Mat& image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // 2^4 hoechster Wert
            return 16;
        case CV_8U:
        case CV_16S:
            // 2^8 hoechster Wert
            return 256;
        case CV_16U:
        case CV_32S:
            // 2^16 hoechster Wert
            return 65536;
        default:
            throw std::runtime_error("Unsupported Mat-type!");
    }
}


#endif //AIOLOS_BASICFUNCTIONS_H
