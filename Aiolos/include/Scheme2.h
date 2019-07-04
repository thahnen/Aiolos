//
// Created by thahnen on 28.06.19.
//

#ifndef AIOLOS_SCHEME2_H
#define AIOLOS_SCHEME2_H

#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>
#include "Standard.h"


namespace Scheme2 {
    /**
     *  The Version as it is defined in the paper per GLCM-item! Very slow but I have no other solution yet!
     *
     *  @param image        the given image
     *  @param m            Matrix element (m,n)
     *  @param n            Matrix element (m,n)
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in degrees!)
     *  @return             the number of pixel pairs which coincide with the requested grayscale value!
     */
    unsigned int G(const cv::Mat& image, unsigned int m, unsigned int n, double r, double theta) {
        unsigned int q = Standard::Q(image, r, theta);
        unsigned int value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (int i = 0; i < image.cols; i++) {
            for (int j = 0; j < image.rows; j++) {
                unsigned int x = i + r*cos(theta);
                unsigned int y = j + r*sin(theta);

                if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                    switch (image.type() & CV_MAT_DEPTH_MASK) {
                        case CV_8SC1:
                            if (image.at<char>(j, i) == m-1 && image.at<char>(y, x) == n-1) value++;
                            break;
                        case CV_8UC1:
                            if (image.at<uchar>(j, i) == m-1 && image.at<uchar>(y, x) == n-1) value++;
                            break;
                        case CV_16SC1:
                            if (image.at<short>(j, i) == m-1 && image.at<short>(y, x) == n-1) value++;
                            break;
                        case CV_16UC1:
                            if (image.at<uchar>(j, i) == m-1 && image.at<uchar>(y, x) == n-1) value++;
                            break;
                        case CV_32SC1:
                            if (image.at<int>(j, i) == m-1 && image.at<int>(y, x) == n-1) value++;
                            break;
                        default:
                            throw std::runtime_error("Unsupported Mat-type!");
                    }
                }
            }
        }
        return value / q;
    }

    /**
     *  Adjusted version for creating a single GLCM used by Scheme 2
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     */
    void GLCM(const cv::Mat& image, cv::Mat_<double>& glcm, double r, double theta) {
        unsigned int dist_x = floor(r*cos(theta));
        unsigned int dist_y = floor(r*sin(theta));

        double c_1 = (dist_x+1 - r*cos(theta)) * (dist_y+1 -r*sin(theta));
        double c_2 = (r*cos(theta) - dist_x) * (dist_y+1 - r*sin(theta));
        double c_3 = (dist_x+1 - r*cos(theta)) * (r*sin(theta) - dist_y);
        double c_4 = (r*cos(theta) - dist_x) * (r*sin(theta) - dist_y);

        #pragma omp parallel for collapse(2)
        for (unsigned int m = 0; m < glcm.cols; m++) {
            for (unsigned int n = 0; n < glcm.rows; n++) {
                unsigned int gray_1 = G(image, m, n, dist_x, dist_y);
                unsigned int gray_2 = G(image, m, n, dist_x+1, dist_y);
                unsigned int gray_3 = G(image, m, n, dist_x, dist_y);
                unsigned int gray_4 = G(image, m, n, dist_x+1, dist_y+1);

                glcm(n, m) = c_1*gray_1 + c_2*gray_2 + c_3*gray_3 + c_4*gray_4;
            }
        }
    }
}


#endif //AIOLOS_SCHEME2_H
