//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_BASICFUNCTIONS_H
#define AIOLOS_BASICFUNCTIONS_H

#include <iostream>
#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>


unsigned int max_gray_value(cv::Mat image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // 2^4 hoechster Wert
            return 16;
        case CV_8U:
        case CV_16S:
            // 2^8 hoechster Wert
            return 256;
        case CV_16U:
        case CV_32S:
            // 2^16 hoechster Wert
            return 65536;
        default:
            // Float etc nicht bedacht, erst einmal das zurückgeben!
            return -1;
    }
}


unsigned int Q(cv::Mat image, double r, unsigned int theta) {
    int value = 0;
    for (int i = 0; i < image.cols; ++i) {
        for (int j = 0; j < image.rows; ++j) {
            unsigned x = i + r*cos(theta);
            unsigned y = j + r*sin(theta);

            if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                value++;
            }
        }
    }
}


unsigned int G(cv::Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) {
    unsigned int q = Q(image, r, theta);
    unsigned int value = 0;


    for (int x1 = 0; x1 < image.cols; ++x1) {
        for (int y1 = 0; y1 < image.rows; ++y1) {
            int x2 = x1 + r*cos(theta);
            int y2 = y1 + r*sin(theta);

            if (x2 >= 0 && x2 < image.cols && y2 >= 0 && y2 < image.cols) {
                uchar val1, val2;
                switch (image.type() & CV_MAT_DEPTH_MASK) {
                    case CV_8SC1:
                        if (image.at<char>(x1, y1) == m && image.at<char>(x2, y2) == n) value++;
                        break;
                    case CV_8UC1:
                        if (image.at<uchar>(y1, x1) == m && image.at<uchar>(y2, x2) == n) value++;
                        break;
                    case CV_16SC1:
                        if (image.at<short>(y1, x1) == m && image.at<short>(y2, x2) == n) value++;
                        break;
                    case CV_16UC1:
                        if (image.at<ushort>(y1, x1) == m && image.at<ushort>(y2, x2) == n) value++;
                        break;
                    case CV_32SC1:
                        if (image.at<int>(y1, x1) == m && image.at<int>(y2, x2) == n) value++;
                        break;
                    default:
                        throw std::runtime_error("Unsupported Mat-type!");
                }
            }
        }
    }

    // return GLCM-Matrix element
    return value / q;
}


#endif //AIOLOS_BASICFUNCTIONS_H
