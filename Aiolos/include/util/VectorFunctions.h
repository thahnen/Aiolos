//
// Created by Tobias Hahnen on 2019-08-06.
//

#pragma once
#ifndef AIOLOS_VECTORFUNCTIONS_H
#define AIOLOS_VECTORFUNCTIONS_H

#include <cmath>
#include <vector>
#include <numeric>
#include "../impl/Enumerations.h"


/**
 *  Returns the value of the given quantile
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values (copy)
 *  @param q            which quantile should be calculated (0.25, 0.5, 0.75)
 *  @return             the requested quantiles value
 *
 *  REVIEW: Assert list is not empty!
 */
template <typename T>
T getQuantileValue(std::vector<T> list, GLCM::Quantil q) {
    assert(list.size() > 0);

    // Sort copied vector
    std::sort(list.begin(), list.end());

    int pos;
    auto len = list.size();

    switch (q) {
        case GLCM::LOWER_QUARTILE:
            pos = ceil(len/4);
            break;
        case GLCM::MIDDLE_QUARTILE:
            pos = ceil(len/2);
            break;
        case GLCM::UPPER_QUARTILE:
            pos = ceil(len/2 + len/4);
            break;
        default:
            throw std::invalid_argument("[getQuantileValue] No valid GLCM::Quantil option!");
    }

    if (len % 2 == 1) return list.at(pos);
    return (list.at(pos) + list.at(pos+1)) / 2;
}


/**
 *  Returns the median value of a vector (0.5 quantile)
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @return             the median value
 */
template <typename T>
T getMedianValue(const std::vector<T>& list) {
    return getQuantileValue(list, GLCM::MIDDLE_QUARTILE);
}


/**
 *  Returns the average value of a vector
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values
 *  @return             the average value
 *
 *  REVIEW: Assert list is not empty!
 */
template <typename T>
T getAverageValue(const std::vector<T>& list) {
    assert(list.size() > 0);

    return std::accumulate(
            list.begin(), list.end(), 0.0/list.size()
    ) / list.size();
}


/**
 *  Returns the indizes of the lowest N values of vector
 *
 *  @tparam T           type of the vector values (for further work)
 *  @param list         the vector of values (copy)
 *  @param n            how many values should be returned
 *  @return             vector with size = n
 *
 *  REVIEW: Assert list is not empty!
 *  REVIEW: Assert n < list.size()!
 *
 *  TODO: setting value to maximum is not neccessary the best idea?
 *  TODO: Parallelize for loop!
 */
template <typename T>
std::vector<unsigned int> getLowestIndizes(std::vector<T> list, unsigned int n) {
    assert(list.size() > 0);
    assert(n < list.size());

    // Get maximum value of copy
    T maximum = *max_element(list.begin(), list.end());

    std::vector<unsigned int> indizes;

    for (unsigned int i = 0; i < n; i++) {
        auto min = min_element(list.begin(), list.end());       // get minimum values index
        indizes.push_back(std::distance(list.begin(), min));    // add index
        list.at(std::distance(list.begin(), min)) = maximum;    // set value at index to maximum (to not find it again)
    }

    return indizes;
}


#endif //AIOLOS_VECTORFUNCTIONS_H