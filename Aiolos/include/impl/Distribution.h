//
// Created by Tobias Hahnen on 2019-08-06.
//

#pragma once
#ifndef AIOLOS_DISTRIBUTION_H
#define AIOLOS_DISTRIBUTION_H

#include "Enumerations.h"
#include "Standard.h"
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
     *
     *  TODO: add "double (*inc)(int, int)" as second parameter to make use of different increasing functions
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
     *  Calculates values for all the given angles of concentration_degree(GLCM) (equals the Z'-function from the paper)
     *
     *  @param image                    the given image
     *  @param angle_distribution       all possible orientation angles
     *  @param impl                     which implementation of the GLCM shall be used
     *  @param max_radius               the given maximum radius
     *  @param begin                    the lowest angle, a GLCM shall be calculated for
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    void calc_angle_dist(const cv::Mat& image, std::vector<double>& angle_distribution, Implementation impl,
                         unsigned int max_radius, unsigned int begin) {
        int max_gray = max_gray_value(image);

        // Outer loop beginning with range.first, ending after range.second
        #pragma omp parallel for
        for (unsigned int theta = begin; theta < begin + angle_distribution.size(); theta++) {
            double theta_rad = theta * CV_PI / 180;
            double value = 0;

            #pragma omp parallel for reduction(+:value)
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
     *  @return                         the filled vector of values (size: range.second - range.first + 1)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    std::vector<double> getAngleDistribution(const cv::Mat& image, Implementation impl, unsigned int max_radius,
                                             const Range& range) {
        std::vector<double> orientation_distribution(range.second - range.first + 1);
        calc_angle_dist(image, orientation_distribution, impl, max_radius, static_cast<unsigned int>(range.first));

#if GLCM_DEBUG_ANGLE_DISTRIBUTION
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
        std::cout << "Winkel " << range.first + i << "°: " << orientation_distribution[i] << std::endl;
    }
#endif

        return orientation_distribution;
    }


    /**
     *  Calculates values for all the given angles of Z(cv::Mat1d&) (equals the Z'-function from the paper)
     *
     *  @tparam T                       single channel type: char/uchar, short/ushort, int
     *  @param image                    the given image
     *  @param angle_distribution       all possible orientation angles
     *  @param impl                     which implementation of the GLCM shall be used
     *  @param max_radius               the given maximum radius
     *  @param begin                    the lowest angle, a GLCM shall be calculated for
     *
     *  REVIEW: Use when Mat-Type is known by compile time!
     */
    template <typename T>
    void calc_angle_dist_(const cv::Mat_<T>& image, std::vector<double>& angle_distribution, Implementation impl,
                          unsigned int max_radius, unsigned int begin) {
        int max_gray = max_gray_value(image);

        // Outer loop beginning with range.first, ending after range.second
        #pragma omp parallel for
        for (unsigned int theta = begin; theta < begin + angle_distribution.size(); theta++) {
            double theta_rad = theta * CV_PI / 180;
            double value = 0;

            #pragma omp parallel for reduction(+:value)
            for (unsigned int r = 1; r <= max_radius; r++) {
                cv::Mat1d glcm(max_gray, max_gray, 0.0);

                // Which implementation of the paper shall be used!
                switch (impl) {
                    case GLCM::SCHEME1:
                        Scheme1::GLCM_(image, glcm, r, theta_rad);
                        break;
                    case GLCM::SCHEME2:
                        Scheme2::GLCM_(image, glcm, r, theta_rad);
                        break;
                    case GLCM::SCHEME3:
                        Scheme3::GLCM_(image, glcm, r, theta_rad);
                        break;
                    case GLCM::STANDARD:
                        Standard::GLCM_(image, glcm, r, theta_rad);
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
     *
     *  REVIEW: Use when Mat-Type is known by compile time!
     */
    std::vector<double> getAngleDistribution_(const cv::Mat& image, Implementation impl, unsigned int max_radius,
                                              const Range& range) {
        std::vector<double> orientation_distribution(range.second - range.first + 1);
        auto begin = static_cast<unsigned int>(range.first);

        switch (image.type() & CV_MAT_DEPTH_MASK) {
            case CV_8SC1:
                calc_angle_dist_((cv::Mat_<char>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_8UC1:
                calc_angle_dist_((cv::Mat_<uchar>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_16SC1:
                calc_angle_dist_((cv::Mat_<short>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_16UC1:
                calc_angle_dist_((cv::Mat_<ushort>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            case CV_32SC1:
                calc_angle_dist_((cv::Mat_<int>&) image, orientation_distribution, impl, max_radius, begin);
                break;
            default:
                throw std::logic_error("[getAngleDistribution_] Unsupported Mat-type!");
        }

#if GLCM_DEBUG_ANGLE_DISTRIBUTION_CT
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
        std::cout << "Winkel " << range.first + i << "°: " << orientation_distribution[i] << std::endl;
    }
#endif

        return orientation_distribution;
    }
}


#endif //AIOLOS_DISTRIBUTION_H
