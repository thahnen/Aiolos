//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_GLCM_H
#define AIOLOS_GLCM_H

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>
#include "BasicFunctions.h"


/***********************************************************************************************************************
 *
 *      ALL FUNCTIONS FOR CREATING GLCMs!
 *      TODO: lower the runtime for only one GLCM from ~4min!
 *
 ***********************************************************************************************************************/


/**
 *  Creates a single "gray level co-occurance matrix" (GLCM)
 *
 *  @param image    the given image, the GLCM is created for
 *  @param r        the radius, from the relative position (radius,theta), coinciding with the pixel
 *  @param theta    the angle, from the relative position (radius,theta), coinciding with the pixel
 *  @return         GLCM, to get the values from!
 */
cv::Mat createGLCM(const cv::Mat& image, double r, unsigned int theta) {
    auto begin = std::chrono::steady_clock::now();

    unsigned int max_gray = max_gray_value(image);
    if (max_gray == -1) {
        throw std::runtime_error("Image type not supported!");
    }

    cv::Mat GLCM(max_gray, max_gray, CV_32S, cv::Scalar(0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < max_gray; ++i) {
        for (int j = 0; j < max_gray; ++j) {
            // For every matrix element get its value:
            GLCM.at<int>(i, j) = G(image, i, j, r, theta);
        }
    }

    std::cout << "GLCM(" << r << "," << theta << ") hat gedauert: "
        << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count() << " Sec" << std::endl;
    return GLCM;
}


/**
 *  Creats all relevant "gray level co-occurance matrix" (GLCM)
 *
 *  @param image    the given image, the GLCMs are created for
 *  @return         Matrix of GLCMs, ordered by (angle,radius), to get the values from!
 */
std::vector<std::vector<cv::Mat>> createAllGLCMs(cv::Mat& image) {
    unsigned int max_angle = 180; // weil Orientierung egal ist!
    unsigned int max_radius = 1; //ceil(sqrt(2)*std::max(image.cols, image.rows));

    std::vector<std::vector<cv::Mat>> allGLCMs;

    #pragma omp parallel for
    for (int theta = 0; theta < max_angle; ++theta) {
        std::vector<cv::Mat> GLCMwithAngle;

        #pragma omp parallel for
        for (int r = 0; r < max_radius; ++r) {
            GLCMwithAngle.push_back(createGLCM(image, r, theta));
        }

        allGLCMs.push_back(GLCMwithAngle);
    }

    return allGLCMs;
}


#endif //AIOLOS_GLCM_H
