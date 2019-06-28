//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_GLCM_H
#define AIOLOS_GLCM_H

#include <iostream>
#include <vector>
#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>
#include "BasicFunctions.h"

#define DEBUG_THETA_MIN false


/**
 *  Just to differentiate the possible implementations from the paper!
 */
enum Implementation {
    STANDARD = 0,
    SCHEME1,
    SCHEME2,
    SCHEME3
};


namespace GLCM {
    /**
     *  Normalization factor for the GLCM based on a condition
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
     *  Creates a single GLCM based on the given parameters
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

                //
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

    /**
     *  Calculates the degree of concentration of a GLCM
     *
     *  @param glcm         the GLCM, to work on
     *  @return             the degree of concentration
     */
    double Z(const cv::Mat& glcm) {
        double value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (int x = 0; x < glcm.cols; x++) {
            for (int y = 0; y < glcm.rows; y++) {
                value += ( pow((y+1)-(x+1), 2) * glcm.at<double>(y, x) );
            }
        }

        return value;
    }

    /**
     *  Calculates values for all the given angles of Z(cv::Mat&)
     *
     *  @param image                        the given image
     *  @param orientation_distribution     the possible orientations
     *  @param max_radius                   the given maximum radius
     *  @param impl                         which implementation of the GLCM shall be used
     */
    void Z_(const cv::Mat& image, std::vector<double>& orientation_distribution, unsigned int max_radius, Implementation impl) {
        //#pragma omp parallel for
        for (unsigned int theta = 0; theta < orientation_distribution.size(); theta++) {
            double value = 0;

            #pragma omp parallel for reduction(+:value)
            for (unsigned int r = 1; r <= max_radius; r++) {
                unsigned int max_gray = max_gray_value(image);
                cv::Mat glcm(max_gray, max_gray, CV_64FC1, cv::Scalar(0));

                // Unterscheiden, welche Implementierung genommen wurde
                switch (impl) {
                    case SCHEME1:
                        //
                        break;
                    case SCHEME2:
                        break;
                    case SCHEME3:
                        break;
                    case STANDARD:
                        GLCM(image, glcm, r, theta);
                        break;
                }

                value += Z(glcm);
            }

            orientation_distribution[theta] = value;
        }
    }

    /**
     *  Calculates the dominant texture orientation of an image
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or if not stated one based on the image boundaries
     *  @return             the dominant angle
     */
    double theta_min(const cv::Mat& image, Implementation impl, unsigned int max_r = 0) {
        unsigned int max_radius = max_r == 0 ? ceil(sqrt(2)*std::max(image.cols, image.rows)) : max_r;

        std::vector<double> orientation_distribution(180);
        Z_(image, orientation_distribution, max_radius, impl);

        double min_value = *min_element(orientation_distribution.begin(), orientation_distribution.end());

#if DEBUG_THETA_MIN
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
            std::cout << "Winkel " << i << "°: " << orientation_distribution[i] << std::endl;
        }
#endif

        // Winkel ist in Bogenmaß, daher Grad = Rad * 180/PI
        return (std::arg(min_value)*180/M_PI);
    }
}


#endif //AIOLOS_GLCM_H
