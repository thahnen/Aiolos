//
// Created by Tobias Hahnen on 2019-08-07.
//


#pragma once
#ifndef AIOLOS_ENUMERATIONS_H
#define AIOLOS_ENUMERATIONS_H

#ifndef __APPLE__
    // on macOS, "std::uint8_t" is included in given files by default (no explicit include necessary)
//#   include <cstdint>
#endif


namespace GLCM {
    /// Type alias for the intervall (of angles)
    typedef std::pair<std::uint8_t,     // lower boundary (included)
                      std::uint8_t>     // upper boundary (included)
            Range;


    /**
     *  Just to differentiate the possible implementations from the paper!
     */
    enum Implementation {
        STANDARD = 0,                   // Standard implementation => some values exist and some do not
        SCHEME1,                        // Scheme 1 => image gets rotated
        SCHEME2,                        // Scheme 2 => interpolation of nearby points
        SCHEME3                         // Scheme 3 => interpolation and summary (?)
    };


    /**
     *  Method to use for getting multiple dominant angles!
     */
    enum Method {
        SPLIT_IMAGE_4 = 0,              // splits the image in smaller images, work on their (one) dominant angle
        TOP_2,                          // just returns the 2 most dominant angles
        TOP_3,                          // just returns the 3 most dominant angles
        MEDIAN,                         // works using the Median
        AVERAGE,                        // works using the average value
        L_QUARTILE                      // works using the lower quartile
    };


    /**
     *  To distinguish the different quantiles! (maybe put in another file with other enums)
     */
    enum Quantil {
        LOWER_QUARTILE = 0,             // 0.25 quantile
        MIDDLE_QUARTILE,                // 0.5 quantile
        UPPER_QUARTILE                  // 0.75 quantile
    };
}


#endif //AIOLOS_ENUMERATIONS_H
