//
// Created by Tobias Hahnen on 2019-08-06.
//


#pragma once
#ifndef AIOLOS_DISTRIBUTION_H
#define AIOLOS_DISTRIBUTION_H

#include "Scheme1.h"
#include "Scheme2.h"
#include "Scheme3.h"


/**
 *  CANNOT SPLIT FILE IN DEFINITION (.h) AND IMPLEMENTATION (.cpp)!
 *  See: https://stackoverflow.com/questions/1724036/splitting-templated-c-classes-into-hpp-cpp-files-is-it-possible
 */


namespace GLCM {
    /**
     *  Calculates the degree of concentration of a GLCM (equals the Z-function from the paper)
     *
     *  @param glcm         the GLCM, to work on
     *  @return             the degree of concentration
     *
     *  NOBUG: Do not change x/y to unsigned => would break everything!
     *  REVIEW: Usage does not depend on specific Mat-Type (CT nor RT)
     */
    double concentration_degree(const cv::Mat1d& glcm) {
        double value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (int x = 0; x < glcm.cols; x++) {
            for (int y = 0; y < glcm.rows; y++) {
                value += ( pow((y+1)-(x+1), 2) * glcm(y, x) );
            }
        }

        return value;
    }


    /**
     *  Calculates the degree of concentration of a GLCM using an increasing function (equals the Z-function from the paper)
     *
     *  @param glcm         the GLCM, to work on
     *  @param inc          the incresing function
     *  @return             the degree of concentration
     *
     *  NOBUG: Do not change x/y to unsigned => would break everything!
     *  REVIEW: Usage does not depend on specific Mat-Type (CT nor RT)
     */
    /*double concentration_degree(const cv::Mat1d& glcm, double (*inc)(int, int)) {
        double value = 0;

        #pragma omp parallel for collapse(2) reduction(+:value)
        for (int x = 0; x < glcm.cols; x++) {
            for (int y = 0; y < glcm.rows; y++) {
                value += ( inc(x+1, y+1) * glcm(y, x) );
            }
        }

        return value;
    }*/


    /**
     *  Calculates values for all the given angles of Z(cv::Mat1d&) (equals the Z'-function from the paper)
     *
     *  @tparam T                       single channel type: char/uchar, short/ushort, int
     *  @param image                    the given image
     *  @param angle_distribution       all possible orientation angles
     *  @param impl                     which implementation of the GLCM shall be used
     *  @param max_radius               the given maximum radius
     *  @param begin                    the lowest angle, a GLCM shall be calculated for
     */
    template <typename T>
    void calc_angle_dist(const cv::Mat_<T>& image, std::vector<double>& angle_distribution, Implementation impl,
                          unsigned int max_radius, unsigned int begin) {
        int max_gray = Util::max_gray_value(image);

        // Outer loop beginning with range.first, ending after range.second
        #pragma omp parallel for shared(max_gray)
        for (unsigned int theta = 0; theta < angle_distribution.size(); theta++) {
            double theta_rad = (begin + theta) * CV_PI / 180;
            double value = 0.0;

            #pragma omp parallel for shared(max_gray, theta_rad) reduction(+:value)
            for (unsigned int r = 1; r <= max_radius; r++) {
                cv::Mat1d glcm(max_gray, max_gray, 0.0);

                // Which implementation of the paper shall be used!
                switch (impl) {
                    case SCHEME1:
                        Scheme1::GLCM(image, glcm, r, theta_rad);
                        break;
                    case SCHEME2:
                        Scheme2::GLCM(image, glcm, r, theta_rad);
                        break;
                    case SCHEME3:
                        Scheme3::GLCM(image, glcm, r, theta_rad);
                        break;
                    case STANDARD:
                        Standard::GLCM(image, glcm, r, theta_rad);
                }

                value += concentration_degree(glcm);
            }

            angle_distribution[theta] = value;
        }
    }


    /**
     *  Calculates the distribution for every angle R1 ... R2 (in degrees)
     *
     *  @param image                    the given image
     *  @param impl                     which implementation of the GLCM shall be used
     *  @param max_radius               the given maximum radius
     *  @param range                    the range, which angles shall be considered
     *  @return                         the filled vector of values  (size: range.second - range.first + 1)
     */
    std::vector<double> getAngleDistribution(const cv::Mat& image, Implementation impl, unsigned int max_radius,
                                              const Range& range) {
        std::vector<double> orientation_distribution(range.second - range.first + 1);
        auto begin = static_cast<unsigned int>(range.first);

        switch (image.type() & CV_MAT_DEPTH_MASK) {
            case CV_8SC1:
                calc_angle_dist((cv::Mat_<char>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_8UC1:
                calc_angle_dist((cv::Mat_<uchar>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_16SC1:
                calc_angle_dist((cv::Mat_<short>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_16UC1:
                calc_angle_dist((cv::Mat_<ushort>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_32SC1:
                calc_angle_dist((cv::Mat_<int>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            default:
                throw std::logic_error("[GLCM::getAngleDistribution_] Unsupported Mat-type!");
        }

#if AIOLOS_DEBUG_ANGLE_DISTRIBUTION
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
        std::cout << "Winkel " << range.first + i << "°: " << orientation_distribution[i] << std::endl;
    }
#endif

        return orientation_distribution;
    }
}


#endif //AIOLOS_DISTRIBUTION_H
