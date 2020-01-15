//
// Created by thahnen on 01.07.19.
//


#pragma once
#ifndef AIOLOS_STANDARD_H
#define AIOLOS_STANDARD_H


namespace GLCM {
    namespace Standard {
        /**
         *  Creates the standard GLCM based on the given parameters
         *
         *  @tparam T           single channel type: char/uchar, short/ushort, int
         *  @param image        the given image
         *  @param glcm         the matrix, the GLCM is stored to
         *  @param r            the radius, the GLCM is based on
         *  @param theta        the angle, the GLCM is based on (in radiant!)
         *
         *  TODO: change x, y to int (and x2, y2 as well) to test for negative values!
         */
        template <typename T>
        void GLCM(const cv::Mat_<T>& image, cv::Mat1d& glcm, double r, double theta) {
            double dist_x = r*cos(theta);
            double dist_y = r*sin(theta);

            #pragma omp parallel for collapse(2) shared(dist_x, dist_y)
            for (unsigned int y = 0; y < image.cols; y++) {
                for (unsigned int x = 0; x < image.rows; x++) {
                    unsigned int x2 = x + dist_x;
                    if (x2 < 0 || x2 >= image.cols) continue;

                    unsigned int y2 = y + dist_y;
                    if (y2 < 0 || y2 >= image.rows) continue;

                    glcm(image(y, x), image(y2, x2))++;
                }
            }
        }
    }
}


#endif //AIOLOS_STANDARD_H
