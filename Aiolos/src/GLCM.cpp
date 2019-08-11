//
// Created by thahnen on 30.07.19.
//

#include <numeric>

#include "util/VectorMath.h"
#include "util/MatrixFunctions.h"
#include "impl/Distribution.h"
#include <GLCM.h>

namespace GLCM {
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
    unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r) {
        return main_angle_range(image, impl, Range(0, 180), max_r);
    }


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
    unsigned int main_angle_range(const cv::Mat& image, GLCM::Implementation impl, const GLCM::Range& range,
                                    unsigned int max_r) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_distribution = getAngleDistribution(image, impl, max_radius, range);

        return range.first + std::distance(
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
     */
    std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, Method meth,
                                            unsigned int max_r) {
        return main_angles_range(image, impl, meth, Range(0, 180), max_r);
    }


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
     *  TODO: Schleifen wenn möglich parallelisieren (ich weiss nicht genau, ob das so richtig ist)
     */
    std::vector<unsigned int> main_angles_range(const cv::Mat& image, GLCM::Implementation impl, Method meth,
                                                    const GLCM::Range& range, unsigned int max_r) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

        std::vector<double> orientation_dist = getAngleDistribution(image, impl, max_radius, Range(0, 180));

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
                    value = getQuantileValue(orientation_dist, GLCM::LOWER_QUARTILE);
                } else {
                    // TODO: noch nicht eingefügte Möglichkeiten bedenken => kommen dann hier rein!
                    throw std::runtime_error("[GLCM::main_angles_range] Other Options not implemented yet!");
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
        unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r) {
            return main_angle_range_(image, impl, Range(0, 180), max_r);
        }


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
         *  REVIEW: Use when Mat-Type is known by compile time!
         */
        unsigned int main_angle_range_(const cv::Mat& image, GLCM::Implementation impl, const GLCM::Range& range,
                                            unsigned int max_r) {
            unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));

            std::vector<double> orientation_distribution = getAngleDistribution_(image, impl, max_radius, range);

#if GLCM_DEBUG_MAIN_ANGLE_CT
            for (unsigned int i = 0; i < orientation_distribution.size(); i++) {
                std::cout << "Winkel " << range.first + i << "°: " << orientation_distribution[i] << std::endl;
            }
#endif

            return std::distance(
                    orientation_distribution.begin(),
                    min_element(orientation_distribution.begin(), orientation_distribution.end())
            );
        }
    }
}