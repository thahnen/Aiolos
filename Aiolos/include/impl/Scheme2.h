//
// Created by thahnen on 28.06.19.
//


#pragma once
#ifndef AIOLOS_SCHEME2_H
#define AIOLOS_SCHEME2_H

#include "Standard.h"


#if __cplusplus > 201402L
namespace GLCM::Scheme2 {
#else
namespace GLCM { namespace Scheme2 {
#endif


    /**
     *  Adjusted version for creating a single GLCM used by Scheme 2
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
        unsigned int dist_x = floor(r*cos(theta));
        unsigned int dist_y = floor(r*sin(theta));

#if AIOLOS_TEST_SCHEME2_GLCM
        // Using this "version": a, b, c, d should only be calculated once
        double c_1, c_2, c_3, c_4;
        {
            double a = (dist_x+1 - r*cos(theta));
            double b = (r*cos(theta) - dist_x);
            double c = (dist_y+1 -r*sin(theta));
            double d = (r*sin(theta) - dist_y);

            c_1 = a*c;
            c_2 = b*c;
            c_3 = a*d;
            c_4 = b*d;
        }
#else
        // Using this "version": c_1-4 are calculated (possible extra calculations)
        double c_1 = (dist_x+1 - r*cos(theta)) * (dist_y+1 -r*sin(theta));
        double c_2 = (r*cos(theta) - dist_x) * (dist_y+1 - r*sin(theta));
        double c_3 = (dist_x+1 - r*cos(theta)) * (r*sin(theta) - dist_y);
        double c_4 = (r*cos(theta) - dist_x) * (r*sin(theta) - dist_y);
#endif

        #pragma omp parallel for collapse(2) shared(dist_x, dist_y, c_1, c_2, c_3, c_4)
        for (unsigned int y = 0; y < image.rows; y++) {
            for (unsigned int x = 0; x < image.cols; x++) {
                // TODO: gibt es nicht einen besseren Weg als einfach der nächste Schleifendurchlauf?
                // TODO: ich weiss noch nicht genau wann und warum gray1-4 addiert grösser sein können als die Anzahl der Spalten!

                unsigned int y1 = y + dist_y;
                if (y1 < 0 || y1 >= image.rows) continue;

                unsigned int x1 = x + dist_x;
                if (x1 < 0 || x1 >= image.cols) continue;

                unsigned int y2 = y + dist_y + 1;
                if (y2 < 0 || y2 >= image.rows) continue;

                unsigned int x2 = x + dist_x + 1;
                if (x2 < 0 || x2 >= image.cols) continue;

                unsigned int gray_1 = c_1 * image(y1, x1);
                unsigned int gray_2 = c_2 * image(y1, x2);
                unsigned int gray_3 = c_3 * image(y2, x1);
                unsigned int gray_4 = c_4 * image(y2, x2);

                if (gray_1+gray_2+gray_3+gray_4 > glcm.cols) {

#if AIOLOS_DEBUG_SCHEME2_GLCM
                    #pragma omp critical
                    {
                        std::cout << "G1-4: " << gray_1+gray_2+gray_3+gray_4
                                    << " , glcm.cols: " << glcm.cols << std::endl;
                    };
#endif

                    continue;
                }

                glcm(image(y, x), gray_1+gray_2+gray_3+gray_4)++;
            }
        }
    }

#if __cplusplus > 201402L
}
#else
}}
#endif


#endif //AIOLOS_SCHEME2_H
