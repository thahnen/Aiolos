//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_BASICFUNCTIONS_H
#define AIOLOS_BASICFUNCTIONS_H

#include <iostream>
#include <chrono>
#include <omp.h>
#include <opencv4/opencv2/opencv.hpp>


unsigned int max_gray_value(const cv::Mat& image) {
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


unsigned int Q(const cv::Mat& image, double r, unsigned int theta) {
    int value = 0;

    #pragma omp parallel for collapse(2)
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


unsigned int G(const cv::Mat& image, unsigned int m, unsigned int n, double r, unsigned int theta) {
    //auto begin = std::chrono::steady_clock::now();

    unsigned int q = Q(image, r, theta);
    unsigned int value = 0;


    #pragma omp parallel for reduction(+:value)
    for (int x1 = 0; x1 < image.cols; ++x1) {
        for (int y1 = 0; y1 < image.rows; ++y1) {
            int x2 = x1 + r*cos(theta);
            int y2 = y1 + r*sin(theta);

            if (x2 >= 0 && x2 < image.cols && y2 >= 0 && y2 < image.cols) {
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

    //std::cout << "G(" << m << "," << n << "," << r << "," << theta << ") hat gedauert: "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << " MilliSec" << std::endl;

    // return GLCM-Matrix element
    return value / q;
}


/***********************************************************************************************************************
 *
 *      BY NOW FOR TESTING PURPOSES ONLY!
 *
 ***********************************************************************************************************************/
 // TODO: maybe change GLCM-matrix to cv::Mat_<double>& ?
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

    for (unsigned i = 0; i < glcm.cols; i++) {
        for (unsigned j = 0; j < glcm.rows; j++) {
            glcm.at<double>(j, i) = glcm.at<double>(j, i) / q;
        }
    }
}

double Z(const cv::Mat& glcm) {
    double value = 0;

    for (int x = 0; x < glcm.cols; x++) {
        for (int y = 0; y < glcm.rows; y++) {
            value = value + ( pow((x+1)-(y+1), 2) * glcm.at<double>(x, y) );
        }
    }

    return value;
}

void Z_(const cv::Mat& image, std::vector<double>& orientation_distribution, unsigned int max_radius) {
    for (unsigned int theta = 0; theta < orientation_distribution.size(); theta++) {
        auto begin = std::chrono::steady_clock::now();

        double value = 0;
        for (unsigned int r = 0; r < max_radius; r++) {

            cv::Mat glcm(max_gray_value(image), max_gray_value(image), CV_64FC1, cv::Scalar(0));
            GLCM(image, glcm, r, theta);
            value = value + Z(glcm);
        }

        orientation_distribution[theta] = value;

        std::cout << "Z_(" << theta << ") hat gedauert: "
                  << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count() << " Sec" << std::endl;
    }
}

unsigned int theta_min(const cv::Mat& image) {
    std::vector<double> orientation_distribution(180);
    unsigned int max_radius = ceil(sqrt(2)*std::max(image.cols, image.rows));

    Z_(image, orientation_distribution, /*max_radius*/ 50);

    for (unsigned int i = 0; i < 180; i++) {
        std::cout << "Winkel " << i << "° => " << orientation_distribution[i] << std::endl;
    }

    unsigned int min_value = *min_element(orientation_distribution.begin(), orientation_distribution.end());

    return std::arg(min_value);
}


#endif //AIOLOS_BASICFUNCTIONS_H
