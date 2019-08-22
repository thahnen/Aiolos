//
// Created by thahnen on 22.08.19.
//


#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <GLCM.h>

using namespace std;
using namespace cv;


/// Outsource this to util/VisualizationHelper
void paintBoxWithAngleAndText(Mat& image, unsigned int main_angle, unsigned int x1, unsigned int x2, unsigned int y1,
                                unsigned int y2, const Scalar& color) {
    unsigned int mx = (x1 + x2) / 2;
    unsigned int my = (y1 + y2) / 2;
    unsigned int len = min(mx, my) / 2;

    // Dominanten Winkel einzeichnen
    Point p1(mx + len*cos(main_angle*CV_PI/180), my + len*sin(main_angle*CV_PI/180));
    Point p2(mx - len*cos(main_angle*CV_PI/180), my - len*sin(main_angle*CV_PI/180));
    line(image, p1, p2, color, 3);

    // Box einzeichnen
    line(image, Point(x1, y1), Point(x2, y1), color, 1);
    line(image, Point(x1, y1), Point(x1, y2), color, 1);
    line(image, Point(x2, y1), Point(x2, y2), color, 1);
    line(image, Point(x1, y2), Point(x2, y2), color, 1);

    // Winkel als Text aufschreiben
    putText(image, to_string(main_angle) + " Grad", Point(x1 + 10, y2 + 30),
            FONT_HERSHEY_SIMPLEX, 1, color, 2);
}


/// Case 1: Get dominant angles through 3x GLCM::CT::main_angle_range_(GLCM::STANDARD)
int main1() {
    Mat image = imread("../../test_videos/keyframes/c.1W.0001.png");    // ausgewählter Bereich: X: 0-510, Y: 55-160
    //Mat image = imread("../../test_videos/keyframes/c.1W.0002.png");    // ausgewählter Bereich: X: 0-510, Y: 55-160
    //Mat image = imread("../../test_videos/keyframes/c.1W.0003.png");    // ausgewählter Bereich: X: 0-510, Y: 50-160
    //Mat image = imread("../../test_videos/keyframes/c.1W.0004.png");    // ausgewählter Bereich: X: 0-510, Y: 55-145
    //Mat image = imread("../../test_videos/keyframes/c.1W.0005.png");    // ausgewählter Bereich: X: 0-510, Y: 55-180

    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    if (gray_image.channels() != 1) {
        cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
        cout << "Image changed to grayscale!" << endl;
    }


    /// Calculate dominant angles
    auto begin = chrono::steady_clock::now();

    int s1_x1 = 0, s1_x2 = 169;
    int s1_y1 = 55, s1_y2 = 160;
    Mat sub1 = gray_image(Range(s1_y1, s1_y2), Range(s1_x1, s1_x2));
    unsigned int s1_main_angle = GLCM::CT::main_angle_range_(sub1, GLCM::STANDARD, GLCM::Range(10, 70), 50);

    int s2_x1 = 170, s2_x2 = 339;
    int s2_y1 = 55, s2_y2 = 160;
    Mat sub2 = gray_image(Range(s2_y1, s2_y2), Range(s2_x1, s2_x2));
    unsigned int s2_main_angle = GLCM::CT::main_angle_range_(sub2, GLCM::STANDARD, GLCM::Range(10, 70), 50);

    int s3_x1 = 340, s3_x2 = 509;
    int s3_y1 = 55, s3_y2 = 160;
    Mat sub3 = gray_image(Range(s3_y1, s3_y2), Range(s3_x1, s3_x2));
    unsigned int s3_main_angle = GLCM::CT::main_angle_range_(sub3, GLCM::STANDARD, GLCM::Range(10, 70), 50);

    cout << "Mehrere dominante Winkel über 3x main_angle (CT, STANDARD, Range(10, 70)) - Dauer: "
            << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Winkel sind (von links nach rechts): "
            << s1_main_angle << "°, " << s2_main_angle << "°, " << s3_main_angle << "°" << endl;


    /// Paint dominant angles and sub image boundaries in image
    paintBoxWithAngleAndText(image, s1_main_angle, s1_x1, s1_x2, s1_y1, s1_y2, Scalar(0, 0, 255));
    paintBoxWithAngleAndText(image, s2_main_angle, s2_x1, s2_x2, s2_y1, s2_y2, Scalar(0, 255, 0));
    paintBoxWithAngleAndText(image, s3_main_angle, s3_x1, s3_x2, s3_y1, s3_y2, Scalar(255, 0, 0));


    imshow("Image (grayscale)", gray_image);
    imshow("Image (with angles)", image);
    waitKey(0);


    string dateiname;
    cout << "Dateinamen zum speichern eingeben:";
    cin >> dateiname;
    imwrite("../../test_videos/keyframes/" + dateiname + ".main_angle_range_.STANDARD.png", image);


    return 0;
}


/// Case 2: Get dominant angles through 3x GLCM::CT::main_angle_range_(GLCM::SCHEME2)
int main() {
    //Mat image = imread("../../test_videos/keyframes/c.1W.0001.png");    // ausgewählter Bereich: X: 0-510, Y: 55-160
    Mat image = imread("../../test_videos/keyframes/c.1W.0002.png");    // ausgewählter Bereich: X: 0-510, Y: 55-160
    //Mat image = imread("../../test_videos/keyframes/c.1W.0003.png");    // ausgewählter Bereich: X: 0-510, Y: 50-160
    //Mat image = imread("../../test_videos/keyframes/c.1W.0004.png");    // ausgewählter Bereich: X: 0-510, Y: 55-145
    //Mat image = imread("../../test_videos/keyframes/c.1W.0005.png");    // ausgewählter Bereich: X: 0-510, Y: 55-180

    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    if (gray_image.channels() != 1) {
        cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
        cout << "Image changed to grayscale!" << endl;
    }


    /// Calculate dominant angles
    auto begin = chrono::steady_clock::now();

    int s1_x1 = 0, s1_x2 = 169;
    int s1_y1 = 55, s1_y2 = 160;
    Mat sub1 = gray_image(Range(s1_y1, s1_y2), Range(s1_x1, s1_x2));
    unsigned int s1_main_angle = GLCM::CT::main_angle_range_(sub1, GLCM::SCHEME2, GLCM::Range(10, 70), 50);

    int s2_x1 = 170, s2_x2 = 339;
    int s2_y1 = 55, s2_y2 = 160;
    Mat sub2 = gray_image(Range(s2_y1, s2_y2), Range(s2_x1, s2_x2));
    unsigned int s2_main_angle = GLCM::CT::main_angle_range_(sub2, GLCM::SCHEME2, GLCM::Range(10, 70), 50);

    int s3_x1 = 340, s3_x2 = 509;
    int s3_y1 = 55, s3_y2 = 160;
    Mat sub3 = gray_image(Range(s3_y1, s3_y2), Range(s3_x1, s3_x2));
    unsigned int s3_main_angle = GLCM::CT::main_angle_range_(sub3, GLCM::SCHEME2, GLCM::Range(10, 70), 50);

    cout << "Mehrere dominante Winkel über 3x main_angle (CT, SCHEME2, Range(10, 70)) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Winkel sind (von links nach rechts): "
         << s1_main_angle << "°, " << s2_main_angle << "°, " << s3_main_angle << "°" << endl;


    /// Paint dominant angles and sub image boundaries in image
    paintBoxWithAngleAndText(image, s1_main_angle, s1_x1, s1_x2, s1_y1, s1_y2, Scalar(0, 0, 255));
    paintBoxWithAngleAndText(image, s2_main_angle, s2_x1, s2_x2, s2_y1, s2_y2, Scalar(0, 255, 0));
    paintBoxWithAngleAndText(image, s3_main_angle, s3_x1, s3_x2, s3_y1, s3_y2, Scalar(255, 0, 0));


    imshow("Image (grayscale)", gray_image);
    imshow("Image (with angles)", image);
    waitKey(0);


    string dateiname;
    cout << "Dateinamen zum speichern eingeben:";
    cin >> dateiname;
    imwrite("../../test_videos/keyframes/" + dateiname + ".main_angle_range_.SCHEME2.png", image);


    return 0;
}