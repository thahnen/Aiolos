//
// Created by Tobias Hahnen on 2019-08-07.
//


#pragma once
#ifndef AIOLOS_ENUMERATIONS_H
#define AIOLOS_ENUMERATIONS_H


namespace GLCM {
    /// Type alias for the range type
    typedef std::pair<std::uint8_t,     // lower boundary (included) in intervall (of angles)
                      std::uint8_t>     // upper boundary (included) in intervall (of angles)
            Range;


    /// Type alias for the feature (debug) vector + their info
    typedef std::vector<std::pair<std::string, std::string>> FEATURES;
    typedef FEATURES DEBUGS;


    /// Just to differentiate the possible implementations from the paper!
    enum Implementation {
        STANDARD = 0,                   // Standard implementation => some values exist and some do not
        SCHEME1,                        // Scheme 1 => image gets rotated
        SCHEME2,                        // Scheme 2 => interpolation of nearby points
        SCHEME3                         // Scheme 3 => interpolation and summary (?)
    };


    /// Method to use for getting multiple dominant angles!
    /// TODO: add more possibilities (given boundaries to work with, ...)
    enum Method {
                                        // splitting the image in smaller ones, work on their (one) dominant angle:
        SPLIT_IMAGE_2x2 = 0,            // 4    ->  2 in width, 2 in height
        SPLIT_IMAGE_3x3,                // 9    ->  3 in width, 3 in height
        SPLIT_IMAGE_4x4,                // 16   ->  4 in width, 4 in height
        SPLIT_IMAGE_1x2,                // 2    ->  1 in width, 2 in height
        SPLIT_IMAGE_1x3,                // 3    ->  1 in width, 3 in height
        SPLIT_IMAGE_1x4,                // 4    ->  1 in width, 4 in height
        SPLIT_IMAGE_2x1,                // 2    ->  2 in width, 1 in height
        SPLIT_IMAGE_3x1,                // 3    ->  3 in width, 1 in height
        SPLIT_IMAGE_4x1,                // 4    ->  4 in width, 1 in height

                                        // statistical methods:
        MEDIAN,                         // works using the Median
        AVERAGE,                        // works using the average value
        L_QUARTILE,                     // works using the lower quartile

                                        // get the top N dominant angles:
        TOP_2,                          // returns the 2 most dominant angles
        TOP_3                           // returns the 3 most dominant angles
    };


    /// To distinguish the different quantiles! (maybe put in another file with other enums)
    enum Quantil {
        LOWER_QUARTILE = 0,             // 0.25 quantile
        MIDDLE_QUARTILE,                // 0.5 quantile
        UPPER_QUARTILE                  // 0.75 quantile
    };
}


#endif //AIOLOS_ENUMERATIONS_H
