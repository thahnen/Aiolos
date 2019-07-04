//
// Created by thahnen on 28.06.19.
//

#ifndef AIOLOS_SCHEME3_H
#define AIOLOS_SCHEME3_H

#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>


namespace Scheme3 {
    /**
     *  Adjusted Normalization factor for the GLCM used by Scheme 3
     *
     *  @param image        the given image, to test the condition
     *  @param r            the radius, part of the condition
     *  @param theta        the angle, part of the condition (in radiant!)
     *  @return             the number of pixel pairs which fullfill the condition
     */
    [[deprecated("Not implemented yet!")]]
    unsigned int Q(const cv::Mat& image, double r, double theta) {
        return 0;
    }

    /**
     *  Adjusted version for creating a single GLCM used by Scheme 3
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     */
    [[deprecated("Not implemented yet!")]]
    void GLCM(const cv::Mat& image, cv::Mat_<double>& glcm, double r, double theta) {
        return;
    }
}


#endif //AIOLOS_SCHEME3_H
