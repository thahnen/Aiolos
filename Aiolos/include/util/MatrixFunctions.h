//
// Created by thahnen on 24.06.19.
//


#pragma once
#ifndef AIOLOS_MATRIXFUNCTIONS_H
#define AIOLOS_MATRIXFUNCTIONS_H

#include <opencv2/opencv.hpp>

#include "impl/Enumerations.h"


#if __cplusplus > 201402L
namespace GLCM::Util {
#else
namespace GLCM { namespace Util {
#endif

    /**
     *  Returns the maximum gray value of the image-matrix
     *
     *  @param image        the given image
     *  @return             the value
     */
    int max_gray_value(const cv::Mat& image);


    /**
     *  Splits image in 4 subimages to evaluate their angles
     *
     *  @param image        the given image
     *  @param angles       the returned vector of found angles
     *  @param impl         which implementation of the GLCM shall be used
     *  @param range        interval of angles to consider!
     *  @param max_r        maximum radius
     */
    void split_image(const cv::Mat& image, std::vector<unsigned int>& angles, Implementation impl, const Range& range,
                        unsigned int max_r);

#if __cplusplus > 201402L
}
#else
}}
#endif


#endif //AIOLOS_MATRIXFUNCTIONS_H
