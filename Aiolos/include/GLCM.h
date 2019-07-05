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
#include "implementations/Standard.h"
#include "implementations/Scheme1.h"
#include "implementations/Scheme2.h"
#include "implementations/Scheme3.h"

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
     *  Calculates the degree of concentration of a GLCM
     *
     *  @param glcm         the GLCM, to work on
     *  @return             the degree of concentration
     *
     *  REVIEW: Usage does not depend on anything
     *  REVIEW: DO NOT CHANGE x/y TO UNSIGNED! BREAKS EVERYTHING I SWEAR!
     */
    double Z(const cv::Mat1d& glcm) {
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
     *  Calculates values for all the given angles of Z(cv::Mat_<double>&)
     *
     *  @param image                        the given image
     *  @param angle_distribution           all possible orientation angles
     *  @param max_radius                   the given maximum radius
     *  @param impl                         which implementation of the GLCM shall be used
     */
    void Z_(const cv::Mat& image, std::vector<double>& angle_distribution, unsigned int max_radius, Implementation impl) {
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

                value += Z(glcm);
            }

            angle_distribution[theta] = value;
        }
    }


    /**
     *  Calculates the dominant texture orientation of an image
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    unsigned int theta_min(const cv::Mat& image, Implementation impl, unsigned int max_r = 0) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_distribution(180);
        Z_(image, orientation_distribution, max_radius, impl);

#if DEBUG_THETA_MIN
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
            std::cout << "Winkel " << i << "°: " << orientation_distribution[i] << std::endl;
        }
#endif

        return std::distance(
                orientation_distribution.begin(),
                min_element(orientation_distribution.begin(), orientation_distribution.end())
        );
    }



    /*******************************************************************************************************************
     *
     *      THIS IS TESING ONLY FOR NOW! USED TO OPTIMIZE COMPILE TIME USAGE
     *
     *******************************************************************************************************************/
    template <typename T>
    void Z__(const cv::Mat_<T>& image, std::vector<double>& angle_distribution, unsigned int max_radius, Implementation impl) {
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

                value += Z(glcm);
            }

            angle_distribution[theta] = value;
        }
    }

    unsigned int theta_min_(const cv::Mat& image, Implementation impl, unsigned int max_r = 0) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_distribution(180);

        switch (image.type() & CV_MAT_DEPTH_MASK) {
            case CV_8SC1:
                Z__((cv::Mat_<char>&) image, orientation_distribution, max_radius, impl);
                break;
            case CV_8UC1:
                Z__((cv::Mat_<uchar>&) image, orientation_distribution, max_radius, impl);
                break;
            case CV_16SC1:
                Z__((cv::Mat_<short>&) image, orientation_distribution, max_radius, impl);
                break;
            case CV_16UC1:
                Z__((cv::Mat_<ushort>&) image, orientation_distribution, max_radius, impl);
                break;
            case CV_32SC1:
                Z__((cv::Mat_<int>&) image, orientation_distribution, max_radius, impl);
                break;
            default:
                throw std::runtime_error("[GLCM::theta_min_] Unsupported Mat-type!");
        }

#if DEBUG_THETA_MIN
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
            std::cout << "Winkel " << i << "°: " << orientation_distribution[i] << std::endl;
        }
#endif

        return std::distance(
                orientation_distribution.begin(),
                min_element(orientation_distribution.begin(), orientation_distribution.end())
        );
    }
}


#endif //AIOLOS_GLCM_H
