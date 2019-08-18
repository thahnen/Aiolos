//
// Created by thahnen on 28.06.19.
//

#pragma once
#ifndef AIOLOS_SCHEME3_H
#define AIOLOS_SCHEME3_H

#include <omp.h>
#include <opencv2/opencv.hpp>


#if __cplusplus > 201402L
namespace GLCM::Scheme3 {
#else
namespace GLCM { namespace Scheme3 {
#endif

    /**
     *  Adjusted Normalization factor for the GLCM used by Scheme 3 (equals the Scheme 3 Q-function from the paper)
     *
     *  @param image        the given image, to test the condition
     *  @param r            the radius, part of the condition
     *  @param theta        the angle, part of the condition (in radiant!)
     *  @return             the number of pixel pairs which fullfill the condition
     *
     *  REVIEW: Usage does not depend on specific Mat-Type
     */
    [[deprecated("[Scheme3::Q] Not implemented yet!")]]
    unsigned int norm(const cv::Mat& image, double r, double theta) {
        throw std::logic_error("[Scheme3::norm] Not implemented yet!");
    }


    /**
     *  Adjusted version for creating a single GLCM used by Scheme 3
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    void GLCM(const cv::Mat& image, cv::Mat1d& glcm, double r, double theta) {
        throw std::logic_error("[Scheme3::GLCM] Not implemented yet!");
    }


    /**
     *  Adjusted version for creating a single GLCM used by Scheme 3
     *
     *  @tparam T           single channel type: char/uchar, short/ushort, int
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     *
     *  REVIEW: Use when T is known by compile time!
     */
    template <typename T>
    void GLCM_(const cv::Mat_<T>& image, cv::Mat1d& glcm, double r, double theta) {
        throw std::logic_error("[Scheme3::GLCM_] Not implemented yet!");
    }

#if __cplusplus > 201402L
}
#else
}}
#endif


#endif //AIOLOS_SCHEME3_H
