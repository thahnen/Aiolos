//
// Created by thahnen on 01.07.19.
//

#ifndef AIOLOS_STANDARD_H
#define AIOLOS_STANDARD_H

#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>


namespace Standard {
    /**
     *  Standard normalization factor for the GLCM based on a condition
     *
     *  @param image        the given image, to test the condition
     *  @param r            the radius, part of the condition
     *  @param theta        the angle, part of the condition (in radiants!)
     *  @return             the number of pixel pairs which fullfill the condition
     */
    unsigned int Q(const cv::Mat& image, double r, double theta) {
        unsigned int value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (unsigned int x = 0; x < image.cols; x++) {
            for (unsigned int y = 0; y < image.rows; y++) {
                unsigned int x2 = x + r*cos(theta);
                unsigned int y2 = y + r*sin(theta);

                if (x2 >= 0 && x2 < image.cols && y2 >= 0 && y2 < image.rows) value++;
            }
        }

        return value;
    }


    /**
     *  Creates the standard GLCM based on the given parameters
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     */
    void GLCM(const cv::Mat& image, cv::Mat_<double>& glcm, double r, double theta) {
        unsigned int q = Q(image, r, theta);

        #pragma omp parallel for collapse(2)
        for (unsigned int y = 0; y < image.cols; y++) {
            for (unsigned int x = 0; x < image.rows; x++) {
                unsigned int y2 = y + r*sin(theta);
                unsigned int x2 = x + r*cos(theta);

                if (x2 < 0 || x2 >= image.cols || y2 < 0 || y2 >= image.rows) continue;

                switch (image.type() & CV_MAT_DEPTH_MASK) {
                    case CV_8SC1:
                        glcm(image.at<char>(y, x), image.at<char>(y2, x2))++;
                        break;
                    case CV_8UC1:
                        glcm(image.at<uchar>(y, x), image.at<uchar>(y2, x2))++;
                        break;
                    case CV_16SC1:
                        glcm(image.at<short>(y, x), image.at<short>(y2, x2))++;
                        break;
                    case CV_16UC1:
                        glcm(image.at<ushort>(y, x), image.at<ushort>(y2, x2))++;
                        break;
                    case CV_32SC1:
                        glcm(image.at<int>(y, x), image.at<int>(y2, x2))++;
                        break;
                    default:
                        throw std::runtime_error("Unsupported Mat-type!");
                }
            }
        }

        #pragma omp parallel for collapse(2)
        for (unsigned int i = 0; i < glcm.cols; i++) {
            for (unsigned int j = 0; j < glcm.rows; j++) {
                glcm(j, i) /= q;
            }
        }
    }
}


#endif //AIOLOS_STANDARD_H
