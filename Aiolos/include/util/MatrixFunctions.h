//
// Created by thahnen on 24.06.19.
//


#pragma once
#ifndef AIOLOS_MATRIXFUNCTIONS_H
#define AIOLOS_MATRIXFUNCTIONS_H

#include <opencv2/opencv.hpp>

#include "impl/Definitions.h"


namespace GLCM {
    namespace Util {
        /**
         *  Returns the number of all possible gray values of the image based on its type
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
         *  @param meth         which of the splitting methods is used!
         *  @param range        interval of angles to consider!
         *  @param max_r        maximum radius
         */
        void split_image(const cv::Mat& image, std::vector<unsigned int>& angles, Implementation impl, Method meth,
                            const Range& range, unsigned int max_r);

    }
}


#endif //AIOLOS_MATRIXFUNCTIONS_H
