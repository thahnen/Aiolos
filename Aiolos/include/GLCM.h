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
#include "Standard.h"
#include "Scheme1.h"
#include "Scheme2.h"
#include "Scheme3.h"

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
     */
    double Z(const cv::Mat_<double>& glcm) {
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
     *  @param orientation_distribution     the possible orientations
     *  @param max_radius                   the given maximum radius
     *  @param impl                         which implementation of the GLCM shall be used
     */
    void Z_(const cv::Mat& image, std::vector<double>& orientation_distribution, unsigned int max_radius, Implementation impl) {
        #pragma omp parallel for
        for (unsigned int theta = 0; theta < orientation_distribution.size(); theta++) {
            double theta_rad = theta * CV_PI / 180;
            double value = 0;

            #pragma omp parallel for reduction(+:value)
            for (unsigned int r = 1; r <= max_radius; r++) {
                unsigned int max_gray = max_gray_value(image);
                cv::Mat_<double> glcm(max_gray, max_gray, 0.0);

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

            orientation_distribution[theta] = value;
        }
    }

    /**
     *  Calculates the dominant texture orientation of an image
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or if not stated one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    unsigned int theta_min(const cv::Mat& image, Implementation impl, unsigned int max_r = 0) {
        unsigned int max_radius = max_r == 0 ? ceil(sqrt(2)*std::max(image.cols, image.rows)) : max_r;

        std::vector<double> orientation_distribution(180);
        Z_(image, orientation_distribution, max_radius, impl);

        int min_value = std::distance(orientation_distribution.begin(), min_element(orientation_distribution.begin(), orientation_distribution.end()));

#if DEBUG_THETA_MIN
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
            std::cout << "Winkel " << i << "°: " << orientation_distribution[i] << std::endl;
        }
#endif

        return min_value;
    }
}


#endif //AIOLOS_GLCM_H
