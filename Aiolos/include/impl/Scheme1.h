//
// Created by thahnen on 28.06.19.
//

#pragma once
#ifndef AIOLOS_SCHEME1_H
#define AIOLOS_SCHEME1_H

#include <omp.h>
#include <opencv2/opencv.hpp>


namespace GLCM {
    namespace Scheme1 {
        /**
         *  Adjusted version for creating a single GLCM used by Scheme 1
         *
         *  @tparam T           single channel type: char/uchar, short/ushort, int
         *  @param image        the given image
         *  @param glcm         the matrix, the GLCM is stored to
         *  @param r            the radius, the GLCM is based on
         *  @param theta        the angle, the GLCM is based on (in radiant!)
         *
         */
        template <typename T>
        void GLCM(const cv::Mat_<T>& image, cv::Mat1d& glcm, double r, double theta) {
            throw std::logic_error("[Scheme1::GLCM_] Not implemented yet!");
        }
    }
}


#endif //AIOLOS_SCHEME1_H
