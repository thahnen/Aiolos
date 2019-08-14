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
     *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
     */
    unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r) {
        return main_angle_range(image, impl, Range(0, 179), max_r);
    }


    /**
     *  Calculates the one dominant texture orientation of an image for specific angles.
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
     *  Calculates the dominant texture orientations of the image (one or more possible).
     */
    std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                            unsigned int max_r) {
        return main_angles_range(image, impl, meth, Range(0, 179), max_r);
    }


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
     *  TODO: Zusammenfassen GLCM::main_angles_range + GLCM::CT::main_angles_range_
     *  TODO: Noch nicht eingefügte Möglichkeiten bedenken!
     */
    std::vector<unsigned int> main_angles_range(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                    const GLCM::Range& range, unsigned int max_r) {
        unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
        std::vector<double> orientation_dist = getAngleDistribution(image, impl, max_radius, range);
        std::vector<unsigned int> angles;

        switch (meth) {
            case SPLIT_IMAGE:
                /* // Test implementation, not final (goes in another file?)
                int m_w = ceil(image.cols/2);
                int m_h = ceil(image.rows/2);

                cv::Mat m1 = image(cv::Range(0, m_h), cv::Range(0, m_w));
                cv::Mat m2 = image(cv::Range(0, m_h), cv::Range(m_w + 1, image.cols));
                cv::Mat m3 = image(cv::Range(m_h + 1, image.rows), cv::Range(0, m_w));
                cv::Mat m4 = image(cv::Range(m_h + 1, image.rows), cv::Range(m_w + 1, image.cols));

                std::set<unsigned int> values;  // set because no double values!
                values.insert(main_angle_range(m1, impl, range, max_radius/2));
                values.insert(main_angle_range(m2, impl, range, max_radius/2));
                values.insert(main_angle_range(m3, impl, range, max_radius/2));
                values.insert(main_angle_range(m4, impl, range, max_radius/2));
                 */

                throw std::logic_error("[GLCM::main_angles_range] SPLIT_IMAGE not implemented yet!");
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
                    throw std::runtime_error("[GLCM::main_angles_range] Other Options not implemented yet!");
                }

                // TODO: Better parallelization -> see: https://stackoverflow.com/a/18671256
                //#pragma omp declare reduction(vector_push_back : \
                //                                std::vector<unsigned int> : \
                //                                omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

                #pragma omp parallel for ordered //reduction(vector_push_back: angles)
                for (auto it = orientation_dist.begin(); it < orientation_dist.end(); ++it) {
                    if (*it < value) {
                        angles.push_back(std::distance(orientation_dist.begin(), it));
                    }
                }
        }

        auto begin = static_cast<unsigned int>(range.first);

        // TODO: Better parallelization -> see: https://stackoverflow.com/a/43169193
        //#pragma omp declare reduction(vector_iterator_add : \
        //                                std::vector<unsigned int> : \
        //                                std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), \
        //                                                omp_out.begin(), std::plus<unsigned int>())) \
        //                                initializer(omp_priv = omp_orig)

        #pragma omp parallel for ordered //reduction(vector_iterator_add: angles)
        for (auto it = angles.begin(); it < angles.end(); ++it) {
            *it += begin;
        }

        return angles;
    }


    namespace CT {
        /**
         *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
         */
        unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r) {
            return main_angle_range_(image, impl, Range(0, 179), max_r);
        }


        /**
         *  Calculates the one dominant texture orientation of an image for specific angles.
         */
        unsigned int main_angle_range_(const cv::Mat& image, GLCM::Implementation impl, const GLCM::Range& range,
                                            unsigned int max_r) {
            unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
            std::vector<double> orientation_distribution = getAngleDistribution_(image, impl, max_radius, range);

            return range.first + std::distance(
                    orientation_distribution.begin(),
                    min_element(orientation_distribution.begin(), orientation_distribution.end())
            );
        }


        /**
         *  Calculates the dominant texture orientations of the image (one or more possible).
         */
        std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                  unsigned int max_r) {
            return main_angles_range_(image, impl, meth, Range(0, 179), max_r);
        }


        /**
         *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
         *  TODO: catch up with GLCM::main_angles_range!
         */
        std::vector<unsigned int> main_angles_range_(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                        const GLCM::Range& range, unsigned int max_r) {
            unsigned int max_radius = max_r != 0 ? max_r : ceil(sqrt(2)*std::max(image.cols/2, image.rows/2));
            std::vector<double> orientation_dist = getAngleDistribution_(image, impl, max_radius, Range(0, 180));
            std::vector<unsigned int> angles;

            switch (meth) {
                case SPLIT_IMAGE:
                    throw std::logic_error("[GLCM::CT::main_angles_range_] SPLIT_IMAGE not implemented yet!");
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
    }
}