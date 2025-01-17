//
// Created by thahnen on 29.07.19.
//

#pragma once
#ifndef AIOLOS_VISUALIZATIONHELPER_H
#define AIOLOS_VISUALIZATIONHELPER_H

#include <opencv2/opencv.hpp>


/**
 *  Can be used to visualize an angle in a given image
 *
 *  @param image                the given image (when grayscale it gets converted to color)
 *  @param main_angle           the given angle to visualize
 *  @param referenceCoords      whether or not a coordinate system should be added
 */
cv::Mat showAngle(const cv::Mat& image, unsigned int main_angle, bool referenceCoords = false) {
    cv::Mat canvas = image.clone();
    if (canvas.channels() == 1) cv::cvtColor(canvas, canvas, cv::COLOR_GRAY2BGR);

    unsigned int middle_x = canvas.cols % 2 == 0 ? canvas.cols/2 : ceil(canvas.cols/2);
    unsigned int middle_y = canvas.rows % 2 == 0 ? canvas.cols/2 : ceil(canvas.rows/2);
    unsigned int length = std::min(middle_x, middle_y) / 2;

    if (referenceCoords) {
        cv::Point m(middle_x, middle_y);

        // Einzeichnen der 0°-Linie, zur Orientierung!
        cv::Point o2(middle_x - length*cos(0), middle_y - length*sin(0));
        line(canvas, m, o2, cv::Scalar(0, 255, 0), 3);

        // Einzeichnen der 90°-Linie, zur Orientierung!
        cv::Point o4(middle_x - length*cos(90*CV_PI/180), middle_y - length*sin(90*CV_PI/180));
        line(canvas, m, o4, cv::Scalar(0, 128, 0), 3);
    }

    // Einzeichnen des dominanten Winkels
    cv::Point p1(middle_x + length*cos(main_angle*CV_PI/180), middle_y + length*sin(main_angle*CV_PI/180));
    cv::Point p2(middle_x - length*cos(main_angle*CV_PI/180), middle_y - length*sin(main_angle*CV_PI/180));
    line(canvas, p1, p2, cv::Scalar(0, 0, 255), 5);

    return canvas;
}


/**
 *  Can be used to visualize multiple angles in a given image
 *
 *  @param image                the given image (when grayscale it gets converted to color)
 *  @param main_angles          a vector of angles to visualize
 *  @param referenceCoords      whether or not a coordinate system should be added
 */
cv::Mat showAngles(const cv::Mat& image, const std::vector<unsigned int>& main_angles, bool referenceCoords = false) {
    cv::Mat canvas = image.clone();
    if (canvas.channels() == 1) cv::cvtColor(canvas, canvas, cv::COLOR_GRAY2BGR);

    unsigned int middle_x = canvas.cols % 2 == 0 ? canvas.cols/2 : ceil(canvas.cols/2);
    unsigned int middle_y = canvas.rows % 2 == 0 ? canvas.rows/2 : ceil(canvas.rows/2);
    unsigned int length = std::min(middle_x, middle_y) / 2;

    if (referenceCoords) {
        cv::Point m(middle_x, middle_y);

        // Einzeichnen der 0°-Linie, zur Orientierung!
        cv::Point o2(middle_x - length*cos(0), middle_y - length*sin(0));
        line(canvas, m, o2, cv::Scalar(0, 255, 0), 3);

        // Einzeichnen der 90°-Linie, zur Orientierung!
        cv::Point o4(middle_x - length*cos(90*CV_PI/180), middle_y - length*sin(90*CV_PI/180));
        line(canvas, m, o4, cv::Scalar(0, 128, 0), 3);
    }

    // TODO: die einzelnen Winkel noch farblich unterscheiden!
    for (unsigned int angle : main_angles) {
        cv::Point p1(middle_x + length*cos(angle*CV_PI/180), middle_y + length*sin(angle*CV_PI/180));
        cv::Point p2(middle_x - length*cos(angle*CV_PI/180), middle_y - length*sin(angle*CV_PI/180));
        line(canvas, p1, p2, cv::Scalar(0, 0, 255), 5);
    }

    return canvas;
}


/// Outsource this to util/VisualizationHelper
void paintBoxWithAngleAndText(cv::Mat& image, unsigned int main_angle, unsigned int x1, unsigned int x2, unsigned int y1,
                              unsigned int y2, const cv::Scalar& color) {
    unsigned int mx = (x1 + x2) / 2;
    unsigned int my = (y1 + y2) / 2;
    unsigned int len = std::min(mx, my) / 2;

    // Dominanten Winkel einzeichnen
    cv::Point p1(mx + len*cos(main_angle*CV_PI/180), my + len*sin(main_angle*CV_PI/180));
    cv::Point p2(mx - len*cos(main_angle*CV_PI/180), my - len*sin(main_angle*CV_PI/180));
    line(image, p1, p2, color, 3);

    // Box einzeichnen
    line(image, cv::Point(x1, y1), cv::Point(x2, y1), color, 1);
    line(image, cv::Point(x1, y1), cv::Point(x1, y2), color, 1);
    line(image, cv::Point(x2, y1), cv::Point(x2, y2), color, 1);
    line(image, cv::Point(x1, y2), cv::Point(x2, y2), color, 1);

    // Winkel als Text aufschreiben
    putText(image, std::to_string(main_angle) + " Grad", cv::Point(x1 + 10, y2 + 30),
            cv::FONT_HERSHEY_SIMPLEX, 1, color, 2);
}


#endif //AIOLOS_VISUALIZATIONHELPER_H
