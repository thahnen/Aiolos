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
 *
 **************************************************************************************************/
int main() {
    //Mat image = imread("/home/thahnen/Downloads/zebrastreifen.jpg");
    //Mat image = imread("/home/thahnen/Downloads/sar_plane.png");
    //Mat image = imread("/home/thahnen/Downloads/sar_landebahn.png");
    //Mat image = imread("/home/thahnen/Downloads/sar_highway.jpg");
    Mat image = imread("/Users/thahnen/Downloads/sar_plane.png");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
    cout << "Graustufenbild mit höchstem Grauwert: " << max_gray_value(gray_image) << endl;


    // Haupt-Orientierung erhalten & Schemata vergleichen!
    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::theta_min(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::theta_min(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    // Vegleich der beiden Möglichkeiten
    cout << "Haupt-Orientierung: Std: " << main_angle << "°, Sc2: " << main_angle2 << "°" << endl;


    // Einzeichnen der 0°-Linie, zur Orientierung!
    Point o1(image.cols/2, image.rows/2);
    Point o2(image.cols/2 - 40*cos(0), image.rows/2 - 40*sin(0));
    line(image, o1, o2, Scalar(0, 255, 0));

    // Einzeichnen der 90°-Linie, zur Orientierung!
    Point o3(image.cols/2, image.rows/2);
    Point o4(image.cols/2 - 40*cos(90*CV_PI/180), image.rows/2 - 40*sin(90*CV_PI/180));
    line(image, o3, o4, Scalar(0, 128, 0));

    // Einzeichnen der Linie, die der vorherrschenden Orientierung folgt!
    Point p1(image.cols/2 + 40*cos(main_angle*CV_PI/180), image.rows/2 + 40*sin(main_angle*CV_PI/180));
    Point p2(image.cols/2 - 40*cos(main_angle*CV_PI/180), image.rows/2 - 40*sin(main_angle*CV_PI/180));
    line(image, p1, p2, Scalar(0, 0, 255), 5);

    imshow("Graubild:", gray_image);
    imshow("Farbbild mit Linie", image);
    waitKey(0);
    return 0;
}
