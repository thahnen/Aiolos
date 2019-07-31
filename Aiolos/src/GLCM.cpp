//
// Created by thahnen on 30.07.19.
//

#include <numeric>
#include <omp.h>

#include "util/BasicFunctions.h"
#include "impl/Standard.h"
#include "impl/Scheme1.h"
#include "impl/Scheme2.h"
#include "impl/Scheme3.h"

#include "GLCM.h"


namespace GLCM {
    namespace {
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
         *
         *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
         */
        void calc_angle_dist(const cv::Mat& image, std::vector<double>& angle_distribution,
                             Implementation impl, unsigned int max_radius) {
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
         *  Calculates for every angle 0-179° the distribution
         *
         *  @param image                    the given image
         *  @param impl                     which implementation of the GLCM shall be used
         *  @param max_radius               the given maximum radius
         *  @return                         the filled vector of values
         */
        std::vector<double> getAngleDistribution(const cv::Mat& image, Implementation impl, unsigned int max_radius) {
            std::vector<double> orientation_distribution(180);
            calc_angle_dist(image, orientation_distribution, impl, max_radius);
            return orientation_distribution;
        }
    }


    /**
     *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper)
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the one dominant angle (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     */
    unsigned int main_angle(const cv::Mat& image, Implementation impl, unsigned int max_r) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_distribution = getAngleDistribution(image, impl, max_radius);

#if GLCM_DEBUG_MAIN_ANGLE
        for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
            std::cout << "Winkel " << i << "°: " << orientation_distribution[i] << std::endl;
        }
#endif

        return std::distance(
                orientation_distribution.begin(),
                min_element(orientation_distribution.begin(), orientation_distribution.end())
        );
    }


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible)
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     *
     *  REVIEW: Use when Mat-Type is not known by compile time -> usage at runtime!
     *  TODO: Schleifen wenn möglich parallelisieren (ich weiss nicht genau, ob das so richtig ist)
     */
    std::vector<unsigned int> main_angles(const cv::Mat& image, Implementation impl, Method meth, unsigned int max_r) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_dist = getAngleDistribution(image, impl, max_radius);

        std::vector<unsigned int> angles;
        switch (meth) {
            case SPLIT_IMAGE:
                throw std::runtime_error("[GLCM::main_angles] SPLIT_IMAGE not implemented yet!");
            case TOP_2:
                angles = getLowestIndizes(orientation_dist, 2);
                break;
            case TOP_3:
                angles = getLowestIndizes(orientation_dist, 3);
                break;
            default:
                double value;

                if (meth == MEDIAN) {
                    value = getMedianValue(orientation_dist);
                } else if (meth == AVERAGE) {
                    value = getAverageValue(orientation_dist);
                } else if (meth == L_QUARTILE) {
                    value = getQuantileValue(orientation_dist, LOWER_QUARTILE);
                } else {
                    // TODO: noch nicht eingefügte Möglichkeiten bedenken => kommen dann hier rein!
                    throw std::runtime_error("[GLCM::main_angles] Other Options not implemented yet!");
                }

                #pragma omp parallel for ordered
                for (auto it = orientation_dist.begin(); it < orientation_dist.end(); ++it) {
                    if (*it < value) {
                        #pragma omp critical
                        {
                            angles.push_back(std::distance(orientation_dist.begin(), it));
                        };
                    }
                }
        }

        return angles;
    }


    namespace CT {
        /**
         *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper)
         *
         *  @param image        the given image
         *  @param impl         which implementation of the GLCM shall be used
         *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
         *  @return             the dominant angle (in degrees!)
         *
         *  REVIEW: Use when Mat-Type is known by compile time!
         */
        unsigned int main_angle(const cv::Mat& image, Implementation impl, unsigned int max_r) {
            unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

            std::vector<double> orientation_distribution(180);

            switch (image.type() & CV_MAT_DEPTH_MASK) {
                case CV_8SC1:
                    calc_angle_dist_((cv::Mat_<char>&) image, orientation_distribution, max_radius, impl);
                    break;
                case CV_8UC1:
                    calc_angle_dist_((cv::Mat_<uchar>&) image, orientation_distribution, max_radius, impl);
                    break;
                case CV_16SC1:
                    calc_angle_dist_((cv::Mat_<short>&) image, orientation_distribution, max_radius, impl);
                    break;
                case CV_16UC1:
                    calc_angle_dist_((cv::Mat_<ushort>&) image, orientation_distribution, max_radius, impl);
                    break;
                case CV_32SC1:
                    calc_angle_dist_((cv::Mat_<int>&) image, orientation_distribution, max_radius, impl);
                    break;
                default:
                    throw std::runtime_error("[GLCM::CT::theta_min] Unsupported Mat-type!");
            }

#if GLCM_DEBUG_MAIN_ANGLE_CT
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
}