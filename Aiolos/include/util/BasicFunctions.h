//
// Created by thahnen on 24.06.19.
//

#ifndef AIOLOS_BASICFUNCTIONS_H
#define AIOLOS_BASICFUNCTIONS_H

#include <opencv2/opencv.hpp>


/**
 *  Returns the maximum gray value of the image
 *
 *  @param image        the given image
 *  @return             the value
 *
 *  REVIEW: Usage does not depend on specific Mat-Type
 *  REVIEW: Assertion only positive values used in image
 */
unsigned int max_gray_value(const cv::Mat& image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // (signed) char    -> 2^7 highest value
            return 128;
        case CV_8U:
            // unsigned char    -> 2^8 highest value
            return 256;
        case CV_16S:
            // (signed) short   -> 2^15 highest value
            return 32768;
        case CV_16U:
            // unsigned short   -> 2^16 highest value
            return 65536;
        case CV_32S:
            // (signed) int     -> 2^31 highest value
            return 2147483648;
        default:
            throw std::runtime_error("[max_gray_value] Unsupported Mat-type!");
    }
}


// TODO: die Datei ggf noch aufsplitten in zwei unterschiedliche (eine fuer Vector-Operationen/ eine für Matrix-Operationen)
/**
 *  To distinguish the different quantiles!
 */
enum Quantil {
    LOWER_QUARTILE = 0,
    MIDDLE_QUARTILE,
    UPPER_QUARTILE
};


/**
 *  Returns the value of the given quantile
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @param q            which quantile should be calculated (0.25, 0.5, 0.75)
 *  @return             the requested quantiles value
 *
 *  TODO: Maybe sorting is not neccessary!
 *  TODO: Assert the list is not empty!
 */
template <typename T>
T getQuantileValue(const std::vector<T>& list, Quantil q) {
    // Copy vector and sort
    std::vector<T> sorted(list);
    std::sort(sorted.begin(), sorted.end());

    int pos;

    auto len = list.size();
    switch (q) {
        case LOWER_QUARTILE:
            pos = len/4;
            if (len % 2 == 1) return sorted.at(pos);
        case MIDDLE_QUARTILE:
            pos = len/2;
        case UPPER_QUARTILE:
            pos = len/2 + len/4;
    }

    if (len % 2 == 1) return sorted.at(pos);
    return (sorted.at(pos) + sorted.at(pos-1)) / 2;
}


/**
 *  Returns the median value of a vector (0.5 quantile)
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @return             the median value
 *
 *  TODO: Assert that list is not empty!
 */
template <typename T>
T getMedianValue(const std::vector<T>& list) {
    return getQuantileValue(list, MIDDLE_QUARTILE);
}


/**
 *  Returns the average value of a vector
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @return             the average value
 *
 *  TODO: Assert that list is not empty!
 */
template <typename T>
T getAverageValue(const std::vector<T>& list) {
    return std::accumulate(
            list.begin(), list.end(), 0.0/list.size()
    ) / list.size();
}


/**
 *  Returns the indizes of the lowest N values of vector
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @param n            how many values should be returned
 *  @return             vector with size = n
 *
 *  TODO: Assert that list is not empty!
 *  TODO: Assert n =< list.size()!
 *  TODO: setting value to maximum is not neccessary the best idea!
 */
template <typename T>
std::vector<unsigned int> getLowestIndizes(const std::vector<T>& list, unsigned int n) {
    // Copy vector to work with
    std::vector<T> copied(list);

    std::vector<unsigned int> indizes;

    for (unsigned int i = 0; i < n; i++) {
        auto min = min_element(copied.begin(), copied.end());                                           // get minimum values index
        indizes.push_back(std::distance(copied.begin(), min));                                          // add index
        copied.at(std::distance(copied.begin(), min)) = *max_element(copied.begin(), copied.end());     // set value at index to maximum (to not find it again)
    }

    return indizes;
}


#endif //AIOLOS_BASICFUNCTIONS_H
