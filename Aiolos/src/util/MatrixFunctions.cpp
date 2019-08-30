//
// Created by Tobias Hahnen on 2019-08-18.
//

#include <limits>

#include "util/MatrixFunctions.h"
#include "GLCM.h"


/**
 *  Returns the maximum gray value of the image-matrix + 1
 *
 *  REVIEW: Usage does not depend on specific Mat-Type
 *  REVIEW: Assert correct maximum values using standard library!
 *
 *  TODO: change to unsigned int again to support CV_32S (?)
 */
int GLCM::Util::max_gray_value(const cv::Mat& image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // (signed) char    -> 2^7 highest value (128)
            return static_cast<int>(std::numeric_limits<char>::max() + 1);
        case CV_8U:
            // unsigned char    -> 2^8 highest value (256)
            return static_cast<int>(std::numeric_limits<unsigned char>::max() + 1);
        case CV_16S:
            // (signed) short   -> 2^15 highest value (32768)
            return static_cast<int>(std::numeric_limits<short>::max() + 1);
        case CV_16U:
            // unsigned short   -> 2^16 highest value (65536)
            return static_cast<int>(std::numeric_limits<unsigned short>::max() + 1);
        /*
        case CV_32S:
            // (signed) int     -> 2^31 highest value (2147483648)
            return std::numeric_limits<int>::max() + 1;
        */
        default:
            throw std::runtime_error("[GLCM::Uitl::max_gray_value] Unsupported Mat-type!");
    }
}


/**
 *  Splits image in subimages to evaluate their angles
 *
 *  TODO: parallelize for loop!
 *  TODO: implement other methods
 */
void GLCM::Util::split_image(const cv::Mat& image, std::vector<unsigned int>& angles, Implementation impl, Method meth,
                 const Range& range, unsigned int max_r) {
    std::vector<cv::Mat> mats;

    if (meth == SPLIT_IMAGE_2x2) {
        int middle_width = ceil(image.cols/2);
        int middle_height = ceil(image.rows/2);

        // First row left to right
        mats.emplace_back(image(cv::Range(0, middle_height), cv::Range(0, middle_width)));
        mats.emplace_back(image(cv::Range(0, middle_height), cv::Range(middle_width + 1, image.cols)));
        // Second row left to right
        mats.emplace_back(image(cv::Range(middle_height + 1, image.rows), cv::Range(0, middle_width)));
        mats.emplace_back(image(cv::Range(middle_height + 1, image.rows), cv::Range(middle_width + 1, image.cols)));
    } else if (meth == SPLIT_IMAGE_3x3) {
        int third_width = ceil(image.cols/3);
        int third_height = ceil(image.rows/3);

        // First row left to right
        mats.emplace_back(image(cv::Range(0, third_height), cv::Range(0, third_width)));
        mats.emplace_back(image(cv::Range(0, third_height), cv::Range(third_width + 1, third_width * 2)));
        mats.emplace_back(image(cv::Range(0, third_height), cv::Range(third_width * 2 + 1, image.cols)));
        // Second row left to right
        mats.emplace_back(image(cv::Range(third_height + 1, third_height * 2), cv::Range(0, third_width)));
        mats.emplace_back(image(cv::Range(third_height + 1, third_height * 2), cv::Range(third_width + 1, third_width * 2)));
        mats.emplace_back(image(cv::Range(third_height + 1, third_height * 2), cv::Range(third_width * 2 + 1, image.cols)));
        // Third row left to right
        mats.emplace_back(image(cv::Range(third_height * 2 + 1, image.rows), cv::Range(0, third_width)));
        mats.emplace_back(image(cv::Range(third_height * 2 + 1, image.rows), cv::Range(third_width + 1, third_width * 2)));
        mats.emplace_back(image(cv::Range(third_height * 2 + 1, image.rows), cv::Range(third_width * 2 + 1, image.cols)));
    } else if (meth == SPLIT_IMAGE_4x4) {
        int fourth_width = ceil(image.cols/4);
        int fourth_height = ceil(image.rows/4);

        // First row left to right
        mats.emplace_back(image(cv::Range(0, fourth_height), cv::Range(0, fourth_width)));
        mats.emplace_back(image(cv::Range(0, fourth_height), cv::Range(fourth_width + 1, fourth_width * 2)));
        mats.emplace_back(image(cv::Range(0, fourth_height), cv::Range(fourth_width * 2 + 1, fourth_width * 3)));
        mats.emplace_back(image(cv::Range(0, fourth_height), cv::Range(fourth_width * 3 + 1, image.cols)));
        // Second row left to right
        mats.emplace_back(image(cv::Range(fourth_height + 1, fourth_height * 2), cv::Range(0, fourth_width)));
        mats.emplace_back(image(cv::Range(fourth_height + 1, fourth_height * 2), cv::Range(fourth_width + 1, fourth_width * 2)));
        mats.emplace_back(image(cv::Range(fourth_height + 1, fourth_height * 2), cv::Range(fourth_width * 2 + 1, fourth_width * 3)));
        mats.emplace_back(image(cv::Range(fourth_height + 1, fourth_height * 2), cv::Range(fourth_width * 3 + 1, image.cols)));
        // Third row left to right
        mats.emplace_back(image(cv::Range(fourth_height * 2 + 1, fourth_height * 3), cv::Range(0, fourth_width)));
        mats.emplace_back(image(cv::Range(fourth_height * 2 + 1, fourth_height * 3), cv::Range(fourth_width + 1, fourth_width * 2)));
        mats.emplace_back(image(cv::Range(fourth_height * 2 + 1, fourth_height * 3), cv::Range(fourth_width * 2 + 1, fourth_width * 3)));
        mats.emplace_back(image(cv::Range(fourth_height * 2 + 1, fourth_height * 3), cv::Range(fourth_width * 3 + 1, image.cols)));
        // Fourth row left to right
        mats.emplace_back(image(cv::Range(fourth_height * 3 + 1, image.rows), cv::Range(0, fourth_width)));
        mats.emplace_back(image(cv::Range(fourth_height * 3 + 1, image.rows), cv::Range(fourth_width + 1, fourth_width * 2)));
        mats.emplace_back(image(cv::Range(fourth_height * 3 + 1, image.rows), cv::Range(fourth_width * 2 + 1, fourth_width * 3)));
        mats.emplace_back(image(cv::Range(fourth_height * 3 + 1, image.rows), cv::Range(fourth_width * 3 + 1, image.cols)));
    } else if (meth == SPLIT_IMAGE_1x2) {
        int middle_height = ceil(image.rows/2);

        mats.emplace_back(image(cv::Range(0, middle_height), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(middle_height + 1, image.rows), cv::Range(0, image.cols)));
    } else if (meth == SPLIT_IMAGE_1x3) {
        int third_height = ceil(image.rows/3);

        mats.emplace_back(image(cv::Range(0, third_height), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(third_height + 1, third_height * 2), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(third_height * 2 + 1, image.rows), cv::Range(0, image.cols)));
    } else if (meth == SPLIT_IMAGE_1x4) {
        int fourth_height = ceil(image.rows/4);

        mats.emplace_back(image(cv::Range(0, fourth_height), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(fourth_height + 1, fourth_height * 2), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(fourth_height * 2 + 1, fourth_height * 3), cv::Range(0, image.cols)));
        mats.emplace_back(image(cv::Range(fourth_height * 3 + 1, image.rows), cv::Range(0, image.cols)));
    } else if (meth == SPLIT_IMAGE_2x1) {
        int middle_width = ceil(image.cols/2);

        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(0, middle_width)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(middle_width + 1, image.cols)));
    } else if (meth == SPLIT_IMAGE_3x1) {
        int third_width = ceil(image.cols/3);

        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(0, third_width)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(third_width + 1, third_width * 2)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(third_width * 2 + 1, image.cols)));
    } else if (meth == SPLIT_IMAGE_4x1) {
        int fourth_width = ceil(image.cols/3);

        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(0, fourth_width)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(fourth_width + 1, fourth_width * 2)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(fourth_width * 2 + 1, fourth_width * 3)));
        mats.emplace_back(image(cv::Range(0, image.rows), cv::Range(fourth_width * 3 + 1, image.cols)));
    } else {
        // TODO: Happens when new and not implemented methods are used!
        throw std::runtime_error("[GLCM::Util::split_image] Unsupported (unimplemented) image splitting method!");
    }

    for (cv::Mat& sub : mats) {
        angles.emplace_back(GLCM::main_angle(sub, impl, range, max_r));
    }
}