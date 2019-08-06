//
// Created by thahnen on 24.06.19.
//

#ifdef __GNUC__
    #define DLL __attribute__ ((visibility("default")))
#elif defined (WIN32)
    #ifdef WIN_EXPORTS
        #define DLL __declspec(dllexport)
    #else
        #define DLL __declspec(dllimport)
    #endif
#else
    #error "No suitable Compiler (GCC or MSVC) used!"
#endif


#ifndef AIOLOS_GLCM_H
#define AIOLOS_GLCM_H

#include <vector>
#include <opencv2/opencv.hpp>


/**
 *  Just to differentiate the possible implementations from the paper!
 */
enum Implementation {
    STANDARD = 0,           // Standard implementation => some values exist and some do not
    SCHEME1,                // Scheme 1 => image gets rotated
    SCHEME2,                // Scheme 2 => interpolation of nearby points
    SCHEME3                 // Scheme 3 => interpolation and summary (?)
};


/**
 *  Method to use for getting multiple dominant angles!
 */
enum Method {
    SPLIT_IMAGE = 0,        // splits the image in smaller images, work on their (one) dominant angle
    TOP_2,                  // just returns the 2 most dominant angles
    TOP_3,                  // just returns the 3 most dominant angles
    MEDIAN,                 // works using the Median
    AVERAGE,                // works using the average value
    L_QUARTILE              // works using the lower quartile
};


namespace GLCM {
    /**
     *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the one dominant angle (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    DLL unsigned int main_angle(const cv::Mat &image, Implementation impl, unsigned int max_r = 0);

    /**
     *  Calculates the one dominant texture orientation of an image.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the one dominant angle (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    DLL unsigned int main_angle_range(const cv::Mat &image, Implementation impl,
                                        std::pair<unsigned int, unsigned int> range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible).
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat &image, Implementation impl, Method meth, unsigned int max_r = 0);

    /**
     *  Calculates the dominant texture orientations of the image (one or more possible).
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    DLL std::vector<unsigned int> main_angles_range(const cv::Mat &image, Implementation impl, Method meth,
                                                    std::pair<unsigned int, unsigned int> range, unsigned int max_r = 0);


    namespace CT {
        /**
         *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
         *
         *  @param image        the given image
         *  @param impl         which implementation of the GLCM shall be used
         *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
         *  @return             the dominant angle (in degrees!)
         *
         *  REVIEW: Use when Mat-Type is known by compile time!
         */
        DLL unsigned int main_angle(const cv::Mat &image, Implementation impl, unsigned int max_r = 0);

        /**
         *  Calculates the dominant texture orientation of an image.
         *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
         *
         *  @param image        the given image
         *  @param impl         which implementation of the GLCM shall be used
         *  @param range        interval of angles to consider!
         *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
         *  @return             the dominant angle (in degrees!)
         *
         *  @REVIEW             Use when Mat-Type is known by compile time!
         *
         *  See also main_angle()
         */
        DLL unsigned int main_angle_range(const cv::Mat &image, Implementation impl,
                                        std::pair<unsigned int, unsigned int> range, unsigned int max_r = 0);
    }
}


#endif //AIOLOS_GLCM_H
