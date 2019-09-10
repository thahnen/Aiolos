//
// Created by thahnen on 24.06.19.
//


#pragma once
#ifndef AIOLOS_GLCM_H
#define AIOLOS_GLCM_H

#if defined (_WIN32) || defined (__CYGWIN__)
#   if defined (WIN_EXPORT)
#       if defined (__GNUC__)
#           define DLL __attribute__ ((dllexport))
#       elif defined (_MSC_VER)
#           define DLL __declspec(dllexport)
#       else
#           error "No suitable Compiler (GCC / MSVC) used (Windows + WIN_EXPORT)!"
#       endif
#   else
#       if defined (__GNUC__)
#           define DLL __attribute__ ((dllimport))
#       elif defined (_MSC_VER)
#           define DLL __declspec(dllimport)
#       else
#           error "No suitable Compiler (GCC / MSVC) used (Windows)!"
#       endif
#   endif
#else
#   if defined (__GNUC__)
#       define DLL __attribute__ ((visibility("default")))
#   else
#       error "No suitable Compiler (GCC) used (non Windows)!"
#   endif
#endif

#include "impl/Enumerations.h"


namespace GLCM {
    /**
     *  Calculates the dominant texture orientation of an image (equals the "min_theta"-function from the paper).
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    DLL unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r = 0);


    /**
     *  Calculates the one dominant texture orientation of an image for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    DLL unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, const GLCM::Range& range,
                                    unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more + duplicates possible).
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more + duplicates possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const GLCM::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible).
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const GLCM::Range& range, unsigned int max_r = 0);



#ifdef AIOLOS_FEATURE_MORE_TYPE_SUPPORT
    /**
     *  Calculates the one dominant texture orientation of an image for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angle(const cv::Mat&, GLCM::Implementation, const GLCM::Range&, unsigned int)
     */
    DLL unsigned int main_angle(const cv::Mat& image, GLCM::Implementation impl, const cv::Range& range,
                                    unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more + duplicates possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angles(const cv::Mat&, GLCM::Implementation, GLCM::Method, const GLCM::Range&, unsigned int)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const cv::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the image (one or more possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angles_set(const cv::Mat&, GLCM::Implementation, GLCM::Method, const GLCM::Range&, unsigned int)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth,
                                                const cv::Range& range, unsigned int max_r = 0);
#endif



#ifdef AIOLOS_FEATURE_SUB_IMAGE
    /**
     *  Calculates the dominant texture orientation of a sub-image (equals the "min_theta"-function from the paper).
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    DLL unsigned int main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                    unsigned int max_r = 0);


    /**
     *  Calculates the one dominant texture orientation of a sub-image for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle (in degrees!)
     */
    DLL unsigned int main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                    const GLCM::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible).
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const GLCM::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more possible).
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @param image        the given image
     *  @param boundaries   the boundaries for a sub image
     *  @param impl         which implementation of the GLCM shall be used
     *  @param meth         which method is used to get the angles
     *  @param range        interval of angles to consider!
     *  @param max_r        fixed maximum radius or, if not stated, one based on the image boundaries
     *  @return             the dominant angle(s) (in degrees!)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const GLCM::Range& range, unsigned int max_r = 0);


#ifdef AIOLOS_FEATURE_MORE_TYPE_SUPPORT
    /**
     *  Calculates the one dominant texture orientation of a sub-image for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angle(const cv::Mat&, const cv::Rect&, GLCM::Implementation, const cv::Range&, unsigned int)
     */
    DLL unsigned int main_angle(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                    const cv::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more + duplicates possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angles(const cv::Mat&, const cv::Rect&, GLCM::Implementation, GLCM::Method, const GLCM::Range&, unsigned int)
     */
    DLL std::vector<unsigned int> main_angles(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const cv::Range& range, unsigned int max_r = 0);


    /**
     *  Calculates the dominant texture orientations of the sub-image (one or more possible) for specific angles.
     *  Range of angles can be restricted by setting range to an interval [A,B] := { x ∈ ℝ | A ≤ x ≤ B }
     *
     *  @see    GLCM::main_angles_set(const cv::Mat&, const cv::Rect&, GLCM::Implementation, GLCM::Method, const GLCM::Range&, unsigned int)
     */
    DLL std::set<unsigned int> main_angles_set(const cv::Mat& image, const cv::Rect& boundaries, GLCM::Implementation impl,
                                                GLCM::Method meth, const cv::Range& range, unsigned int max_r = 0);
#endif
#endif



    /**
     *  Returns a list of all the preprocessor macros the library is compiled with
     *  => which features can be expected from the library
     *  => possible tests for future (or other implementations)
     *
     *  @return             vector with all feature macros set as string (can be empty)
     */
    DLL FEATURES getFeatures();


    /**
     *  Returns a list of all the preprocessor macros the library is compiled with
     *  => which debug symbols are used to debug the functions
     *
     *  @return             vector with all debug symbols set as string (can be empty)
     */
    DLL DEBUGS getDebugs();
}


#endif //AIOLOS_GLCM_H
