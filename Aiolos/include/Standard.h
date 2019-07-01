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
     *  @param theta        the angle, part of the condition
     *  @return             the number of pixel pairs which fullfill the condition
     */
    unsigned int Q(const cv::Mat& image, double r, unsigned int theta) {
        int value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (unsigned x = 0; x < image.cols; x++) {
            for (unsigned int y = 0; y < image.rows; y++) {
                unsigned x2 = x + r*cos(theta);
                unsigned y2 = y + r*sin(theta);

                if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                    value++;
                }
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
     *  @param theta        the angle, the GLCM is based on
     *
     *  TODO: maybe change GLCM-matrix to cv::Mat_<double>& ?
     */
    void GLCM(const cv::Mat& image, cv::Mat& glcm, double r, unsigned int theta) {
        double q = Q(image, r, theta);

        #pragma omp parallel for collapse(2)
        for (unsigned int x = 0; x < image.cols; x++) {
            for (unsigned int y = 0; y < image.rows; y++) {
                int x2 = x + r*cos(theta);
                int y2 = y + r*sin(theta);

                if (x2 < 0 || x2 >= image.cols || y2 < 0 || y2 >= image.rows) continue;

                switch (image.type() & CV_MAT_DEPTH_MASK) {
                    case CV_8SC1:
                        glcm.at<double>(image.at<char>(y, x), image.at<char>(y2, x2)) += 1;
                        break;
                    case CV_8UC1:
                        glcm.at<double>(image.at<uchar>(y, x), image.at<uchar>(y2, x2)) += 1;
                        break;
                    case CV_16SC1:
                        glcm.at<double>(image.at<short>(y, x), image.at<short>(y2, x2)) += 1;
                        break;
                    case CV_16UC1:
                        glcm.at<double>(image.at<ushort>(y, x), image.at<ushort>(y2, x2)) += 1;
                        break;
                    case CV_32SC1:
                        glcm.at<double>(image.at<int>(y, x), image.at<int>(y2, x2)) += 1;
                        break;
                    default:
                        throw std::runtime_error("Unsupported Mat-type!");
                }
            }
        }

        #pragma omp parallel for collapse(2)
        for (unsigned i = 0; i < glcm.cols; i++) {
            for (unsigned j = 0; j < glcm.rows; j++) {
                glcm.at<double>(j, i) = glcm.at<double>(j, i) / q;
            }
        }
    }
}


#endif //AIOLOS_STANDARD_H
