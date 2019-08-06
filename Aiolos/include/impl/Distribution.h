//
// Created by Tobias Hahnen on 2019-08-06.
//

#ifndef AIOLOS_DISTRIBUTION_H
#define AIOLOS_DISTRIBUTION_H

#include "GLCM.h"
#include "Standard.h"
#include "Scheme1.h"
#include "Scheme2.h"
#include "Scheme3.h"


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
    #pragma omp parallel for
    for (unsigned int theta = begin; theta < begin + angle_distribution.size(); theta++) {
        double theta_rad = theta * CV_PI / 180;
        double value = 0;

        #pragma omp parallel for reduction(+:value)
        for (unsigned int r = 1; r <= max_radius; r++) {
            unsigned int max_gray = max_gray_value(image);
            cv::Mat1d glcm(max_gray, max_gray, 0.0);

            // Unterscheiden, welche Implementierung genommen wurde
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
                    break;
            }

            value += concentration_degree(glcm);
        }

        angle_distribution[theta] = value;
    }
}


/**
 *  Calculates values for all the given angles of Z(cv::Mat1d&) (equals the Z'-function from the paper)
 *
 *  @tparam T                       single channel type: char/uchar, short/ushort, int
 *  @param image                    the given image
 *  @param angle_distribution       all possible orientation angles
 *  @param max_radius               the given maximum radius
 *  @param impl                     which implementation of the GLCM shall be used
 *
 *  REVIEW: Use when Mat-Type is known by compile time!
 */
template <typename T>
void calc_angle_dist_(const cv::Mat_<T>& image, std::vector<double>& angle_distribution,
                      unsigned int max_radius, Implementation impl) {
    #pragma omp parallel for
    for (unsigned int theta = 0; theta < angle_distribution.size(); theta++) {
        double theta_rad = theta * CV_PI / 180;
        double value = 0;

        #pragma omp parallel for reduction(+:value)
        for (unsigned int r = 1; r <= max_radius; r++) {
            unsigned int max_gray = max_gray_value(image);
            cv::Mat1d glcm(max_gray, max_gray, 0.0);

            // Unterscheiden, welche Implementierung genommen wurde
            switch (impl) {
                case SCHEME1:
                    Scheme1::GLCM_(image, glcm, r, theta_rad);
                    break;
                case SCHEME2:
                    Scheme2::GLCM_(image, glcm, r, theta_rad);
                    break;
                case SCHEME3:
                    Scheme3::GLCM_(image, glcm, r, theta_rad);
                    break;
                case STANDARD:
                    Standard::GLCM_(image, glcm, r, theta_rad);
                    break;
            }

            value += concentration_degree(glcm);
        }

        angle_distribution[theta] = value;
    }
}


/**
 *  Calculates the distribution for every angle R1 ... R2 °
 *
 *  @param image                    the given image
 *  @param impl                     which implementation of the GLCM shall be used
 *  @param max_radius               the given maximum radius
 *  @param range                    the range, which angles shall be considered
 *  @return                         the filled vector of values
 */
std::vector<double> getAngleDistribution(const cv::Mat& image, Implementation impl,
                                            unsigned int max_radius, const GLCM::Range& range) {
    std::vector<double> orientation_distribution(range.second - range.first);
    calc_angle_dist(image, orientation_distribution, impl, max_radius, (unsigned int)range.first);
    return orientation_distribution;
}


#endif //AIOLOS_DISTRIBUTION_H
