//
// Created by thahnen on 28.06.19.
//

#ifndef AIOLOS_SCHEME1_H
#define AIOLOS_SCHEME1_H

#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>


namespace Scheme1 {
    /**
     *  Adjusted version for creating a single GLCM used by Scheme 1
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    [[deprecated("[Scheme1::GLCM] Not implemented yet!")]]
    void GLCM(const cv::Mat& image, cv::Mat1d& glcm, double r, double theta) {
        return;
    }


    /**
     *  Adjusted version for creating a single GLCM used by Scheme 1
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
    [[deprecated("[Scheme3::GLCM_] Not implemented yet!")]]
    void GLCM_(const cv::Mat_<T>& image, cv::Mat1d& glcm, double r, double theta) {
        return;
    }
}


#endif //AIOLOS_SCHEME1_H
