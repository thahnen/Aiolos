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
     *
     *  TODO: kann eigentlich weg!
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
     *
     *  TODO: ggf dem Image ein Mat_<T> geben mit Template? dann braucht man kein Switch-Case!
     *  TODO: die Koordinaten zusammenfassen, spart Speicher!
     *  TODO: optimieren
     */
    void GLCM(const cv::Mat& image, cv::Mat_<double>& glcm, double r, double theta) {
        unsigned int dist_x = floor(r*cos(theta));
        unsigned int dist_y = floor(r*sin(theta));

        double c_1 = (dist_x+1 - r*cos(theta)) * (dist_y+1 -r*sin(theta));
        double c_2 = (r*cos(theta) - dist_x) * (dist_y+1 - r*sin(theta));
        double c_3 = (dist_x+1 - r*cos(theta)) * (r*sin(theta) - dist_y);
        double c_4 = (r*cos(theta) - dist_x) * (r*sin(theta) - dist_y);

        #pragma omp parallel for collapse(2)
        for (unsigned int y = 0; y < image.rows; y++) {
            for (unsigned int x = 0; x < image.cols; x++) {
                unsigned int y1 = y + dist_y;
                unsigned int x1 = x + dist_x;

                // TODO: gibt es nicht einen besseren Weg?
                if (x1 < 0 || x1 >= image.cols || y1 < 0 || y1 >= image.rows) continue;

                unsigned int y2 = y + dist_y;
                unsigned int x2 = x + dist_x + 1;

                // TODO: gibt es nicht einen besseren Weg?
                if (x2 < 0 || x2 >= image.cols || y2 < 0 || y2 >= image.rows) continue;

                unsigned int y3 = y + dist_y + 1;
                unsigned int x3 = x + dist_x;

                // TODO: gibt es nicht einen besseren Weg?
                if (x3 < 0 || x3 >= image.cols || y3 < 0 || y3 >= image.rows) continue;

                unsigned int y4 = y + dist_y + 1;
                unsigned int x4 = x + dist_x + 1;

                // TODO: gibt es nicht einen besseren Weg?
                if (x4 < 0 || x4 >= image.cols || y4 < 0 || y4 >= image.rows) continue;

                unsigned int gray_1, gray_2, gray_3, gray_4;
                switch (image.type() & CV_MAT_DEPTH_MASK) {
                    case CV_8SC1:
                        gray_1 = c_1 * image.at<char>(y1, x1);
                        gray_2 = c_2 * image.at<char>(y2, x2);
                        gray_3 = c_3 * image.at<char>(y3, x3);
                        gray_4 = c_4 * image.at<char>(y4, x4);

                        glcm(image.at<char>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_8UC1:
                        gray_1 = c_1 * image.at<uchar>(y1, x1);
                        gray_2 = c_2 * image.at<uchar>(y2, x2);
                        gray_3 = c_3 * image.at<uchar>(y3, x3);
                        gray_4 = c_4 * image.at<uchar>(y4, x4);

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {
                            /*
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4 << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                             */
                            continue;
                        }

                        glcm(image.at<uchar>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_16SC1:
                        gray_1 = c_1 * image.at<short>(y1, x1);
                        gray_2 = c_2 * image.at<short>(y2, x2);
                        gray_3 = c_3 * image.at<short>(y3, x3);
                        gray_4 = c_4 * image.at<short>(y4, x4);


                        glcm(image.at<short>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_16UC1:
                        gray_1 = c_1 * image.at<ushort>(y1, x1);
                        gray_2 = c_2 * image.at<ushort>(y2, x2);
                        gray_3 = c_3 * image.at<ushort>(y3, x3);
                        gray_4 = c_4 * image.at<ushort>(y4, x4);

                        glcm(image.at<ushort>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_32SC1:
                        gray_1 = c_1 * image.at<int>(y1, x1);
                        gray_2 = c_2 * image.at<int>(y2, x2);
                        gray_3 = c_3 * image.at<int>(y3, x3);
                        gray_4 = c_4 * image.at<int>(y4, x4);

                        glcm(image.at<int>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    default:
                        throw std::runtime_error("Unsupported Mat-type!");
                }
            }
        }
    }
}


#endif //AIOLOS_SCHEME2_H
