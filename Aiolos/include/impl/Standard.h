//
// Created by thahnen on 01.07.19.
//


#pragma once
#ifndef AIOLOS_STANDARD_H
#define AIOLOS_STANDARD_H


#if __cplusplus > 201402L
namespace GLCM::Standard {
#else
namespace GLCM { namespace Standard {
#endif

    /**
     *  Standard normalization factor for the GLCM based on a condition (equals the standard Q-function from the paper)
     *
     *  @param image        the given image, to test the condition
     *  @param r            the radius, part of the condition
     *  @param theta        the angle, part of the condition (in radiants!)
     *  @return             the number of pixel pairs which fullfill the condition
     *
     *  REVIEW: Usage does not depend on specific Mat-Type
     *  TODO: change x, y to int (and x2, y2 as well) to test for negative values!
     */
    /*unsigned int norm(const cv::Mat& image, double r, double theta) {
        unsigned int value = 0;

        double dist_x = r*cos(theta);
        double dist_y = r*sin(theta);

        #pragma omp parallel for collapse(2) shared(dist_x, dist_y) reduction(+:value)
        for (unsigned int x = 0; x < image.cols; x++) {
            for (unsigned int y = 0; y < image.rows; y++) {
                unsigned int x2 = x + dist_x;
                if (x2 < 0 || x2 >= image.cols) continue;

                unsigned int y2 = y + dist_y;
                if (y2 < 0 || y2 >= image.rows) continue;

                value++;
            }
        }

        return value;
    }*/


    /**
     *  Creates the standard GLCM based on the given parameters
     *
     *  @tparam T           single channel type: char/uchar, short/ushort, int
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     *
     *  TODO: change x, y to int (and x2, y2 as well) to test for negative values!
     */
    template <typename T>
    void GLCM(const cv::Mat_<T>& image, cv::Mat1d& glcm, double r, double theta) {
        double dist_x = r*cos(theta);
        double dist_y = r*sin(theta);

        #pragma omp parallel for collapse(2) shared(dist_x, dist_y)
        for (unsigned int y = 0; y < image.cols; y++) {
            for (unsigned int x = 0; x < image.rows; x++) {
                unsigned int x2 = x + dist_x;
                if (x2 < 0 || x2 >= image.cols) continue;

                unsigned int y2 = y + dist_y;
                if (y2 < 0 || y2 >= image.rows) continue;

                glcm(image(y, x), image(y2, x2))++;
            }
        }
    }

#if __cplusplus > 201402L
}
#else
}}
#endif


#endif //AIOLOS_STANDARD_H
