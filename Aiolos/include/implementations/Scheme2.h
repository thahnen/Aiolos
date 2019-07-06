//
// Created by thahnen on 28.06.19.
//

#ifndef AIOLOS_SCHEME2_H
#define AIOLOS_SCHEME2_H

#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>
#include "Standard.h"

#define DEBUG_GLCM_SCHEME_2     false
#define DEBUG_GLCM_CT_SCHEME_2  false


namespace Scheme2 {
    /**
     *  Adjusted version for creating a single GLCM used by Scheme 2
     *
     *  @param image        the given image
     *  @param glcm         the matrix, the GLCM is stored to
     *  @param r            the radius, the GLCM is based on
     *  @param theta        the angle, the GLCM is based on (in radiant!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    void GLCM(const cv::Mat& image, cv::Mat1d& glcm, double r, double theta) {
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

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                            #if DEBUG_GLCM_SCHEME_2
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                            << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                            #endif

                            continue;
                        }

                        glcm(image.at<char>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_8UC1:
                        gray_1 = c_1 * image.at<uchar>(y1, x1);
                        gray_2 = c_2 * image.at<uchar>(y2, x2);
                        gray_3 = c_3 * image.at<uchar>(y3, x3);
                        gray_4 = c_4 * image.at<uchar>(y4, x4);

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                            #if DEBUG_GLCM_SCHEME_2
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                            << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                            #endif

                            continue;
                        }

                        glcm(image.at<uchar>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_16SC1:
                        gray_1 = c_1 * image.at<short>(y1, x1);
                        gray_2 = c_2 * image.at<short>(y2, x2);
                        gray_3 = c_3 * image.at<short>(y3, x3);
                        gray_4 = c_4 * image.at<short>(y4, x4);

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                            #if DEBUG_GLCM_SCHEME_2
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                            << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                            #endif

                            continue;
                        }

                        glcm(image.at<short>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_16UC1:
                        gray_1 = c_1 * image.at<ushort>(y1, x1);
                        gray_2 = c_2 * image.at<ushort>(y2, x2);
                        gray_3 = c_3 * image.at<ushort>(y3, x3);
                        gray_4 = c_4 * image.at<ushort>(y4, x4);

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                            #if DEBUG_GLCM_SCHEME_2
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                            << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                            #endif

                            continue;
                        }

                        glcm(image.at<ushort>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    case CV_32SC1:
                        gray_1 = c_1 * image.at<int>(y1, x1);
                        gray_2 = c_2 * image.at<int>(y2, x2);
                        gray_3 = c_3 * image.at<int>(y3, x3);
                        gray_4 = c_4 * image.at<int>(y4, x4);

                        if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                            #if DEBUG_GLCM_SCHEME_2
                            #pragma omp critical
                            {
                                std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                            << " , glcm.cols: " << glcm.cols << std::endl;
                            };
                            #endif

                            continue;
                        }

                        glcm(image.at<int>(y, x), gray_1+gray_2+gray_3+gray_4)++;
                        break;
                    default:
                        throw std::runtime_error("[Scheme2::GLCM] Unsupported Mat-type!");
                }
            }
        }

        // TODO: Division by Q is not really neccessary!? Numbers only get smaller?
        unsigned int q = Standard::Q(image, r, theta);

        #pragma omp parallel for collapse(2)
        for (unsigned int i = 0; i < glcm.cols; i++) {
            for (unsigned int j = 0; j < glcm.rows; j++) {
                glcm(j, i) /= q;
            }
        }
    }


    /**
     *  Adjusted version for creating a single GLCM used by Scheme 2
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

                unsigned int gray_1 = c_1 * image(y1, x1);
                unsigned int gray_2 = c_2 * image(y2, x2);
                unsigned int gray_3 = c_3 * image(y3, x3);
                unsigned int gray_4 = c_4 * image(y4, x4);

                if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

                    #if DEBUG_GLCM_CT_SCHEME_2
                    #pragma omp critical
                    {
                        std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4 << " , glcm.cols: " << glcm.cols << std::endl;
                    };
                    #endif

                    continue;
                }

                glcm(image(y, x), gray_1+gray_2+gray_3+gray_4)++;
            }
        }

        // TODO: Division by Q is not really neccessary!? Numbers only get smaller?
        unsigned int q = Standard::Q(image, r, theta);

        #pragma omp parallel for collapse(2)
        for (unsigned int i = 0; i < glcm.cols; i++) {
            for (unsigned int j = 0; j < glcm.rows; j++) {
                glcm(j, i) /= q;
            }
        }
    }
}


#endif //AIOLOS_SCHEME2_H
