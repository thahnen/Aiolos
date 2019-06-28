#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "GLCM.h"

using namespace std;
using namespace cv;


/**************************************************************************************************
 *
 *      MAIN FUNCTION FOR TESTING
 *      TODO: does run infinitly?
 *
 **************************************************************************************************/
int main() {
    //Mat image = imread("/home/thahnen/Downloads/zebrastreifen.jpg");
    Mat image = imread("/home/thahnen/Downloads/sar_plane.png");

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
    cout << "Graustufenbild mit höchstem Grauwert: " << max_gray_value(gray_image) << endl;

    // GLCM testweise erstellen
    auto begin = chrono::steady_clock::now();
    double main_angle = GLCM::theta_min(gray_image, STANDARD, 50);
    cout << "Haupt-Orientierung: " << main_angle << "°" << endl;
    cout << "Hat insgesamt gedauert: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;


    // Einzeichnen der Linie, die der vorherrschenden Orientierung folgt!
    Point p1(image.cols/2 + 40*cos(main_angle), image.rows/2 + 40*sin(main_angle));
    Point p2(image.cols/2 - 40*cos(main_angle), image.rows/2 - 40*sin(main_angle));
    line(image, p1, p2, Scalar(0, 0, 255));

    imshow("Graubild:", gray_image);
    imshow("Farbbild mit Linie", image);
    waitKey(0);
    return 0;
}
