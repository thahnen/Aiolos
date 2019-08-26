#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <GLCM.h>

#include "util/VisualizationHelper.h"

using namespace std;
using namespace cv;


/**************************************************************************************************
 *
 *      MAIN 1) TESTING DIFFERENCES STANDARD + SCHEME 2 ON EVERY IMAGE
 *
 **************************************************************************************************/
int main1() {
    cout << "OpenCV-Version: " << CV_VERSION << endl;

    // Sea 400x400
    cout << "\nSea 400x400" << endl;
    Mat image = imread("../../test_images/sea_400x400.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle2 << "°" << endl;

    std::vector<unsigned int> values{main_angle, main_angle2};
    showAngles(image, values, true);
    waitKey(0);


    // Zebrastreifen
    cout << "\nZebrastreifen" << endl;
    image = imread("../../test_images/zebrastreifen.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    std::vector<unsigned int> values2{main_angle, main_angle2};
    showAngles(image, values2, true);
    waitKey(0);


    // SAR Flugzeug
    cout << "\nSAR Flugzeug" << endl;
    image = imread("../../test_images/sar_plane.png");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    std::vector<unsigned int> values3{main_angle, main_angle2};
    showAngles(image, values3, true);
    waitKey(0);


    // SAR Landebahn
    cout << "\nSAR Landebahn" << endl;
    image = imread("../../test_images/sar_landebahn.png");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    std::vector<unsigned int> values4{main_angle, main_angle2};
    showAngles(image, values4, true);
    waitKey(0);


    // SAR Highway
    cout << "\nSAR Highway" << endl;
    image = imread("../../test_images/sar_highway.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    std::vector<unsigned int> values5{main_angle, main_angle2};
    imshow("Main angles:", showAngles(image, values5, true));
    waitKey(0);

    return 0;
}


/***********************************************************************************************************************
 *
 *      MAIN 2) TESTING DIFFERENT IMPLEMENTATIONS OF STANDARD + SCHEME 2 (OPTIMIZING)
 *
 ***********************************************************************************************************************/
int main2() {
    cout << "OpenCV-Version: " << CV_VERSION << endl;
    cout << "OpenMP-Version: " << _OPENMP << endl;
    cout << "GCC-Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << endl;

    //Mat image = imread("../../test_images/maps_texel_sea.png");
    //Mat image = imread("../../test_images/sar_highway.jpg");
    //Mat image = imread("../../test_images/sar_landebahn.png");
    //Mat image = imread("../../test_images/sea_400x400.jpg");
    Mat image = imread("../../test_images/zebrastreifen.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }
    cout << "Image loaded!" << endl;

    Mat gray_image = image.clone();
    if (gray_image.channels() != 1) {
        cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
        cout << "Image changed to grayscale!" << endl;
    }

    // Haupt-Orientierung erhalten & Schemata vergleichen!
    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    // Vegleich der beiden Möglichkeiten
    cout << "Haupt-Orientierung: Std: " << main_angle  << "°" << endl;
}


/***********************************************************************************************************************
 *
 *      MAIN 3) TESTING ON ACTUAL DATA (VIDEO)
 *
 ***********************************************************************************************************************/
int main() {
    GLCM::FEATURES f = GLCM::getFeatures();
    cout << "Aiolos library features: " << f.size() << endl;
    if (f.size() > 0) {
        for (auto ff : f) {
            cout << "Feature: " << ff.first << " -> " << ff.second << endl;
        }
    }

    GLCM::DEBUGS d = GLCM::getDebugs();
    cout << "Aioloos library debug symbols: " << d.size() << endl;
    if (d.size() > 0) {
        for (auto dd : d) {
            cout << "Debug symbol: " << dd.first << " -> " << dd.second << endl;
        }
    }

    VideoCapture cap("../../test_videos/c.1W.avi");
    //VideoCapture cap("../../test_videos/c.2R.avi");
    if (!cap.isOpened()) {
        cerr << "Video kann nicht wiedergegeben werden!" << endl;
        return 1;
    }

    Mat frame;
    for (;;) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "Frame ist leer" << endl;
            break;
        }

        if (frame.channels() != 1) cvtColor(frame, frame, COLOR_BGR2GRAY);

        auto begin = chrono::steady_clock::now();
        unsigned int main_angle = GLCM::main_angle(frame, GLCM::STANDARD, GLCM::Range(10, 70), 50);
        cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

        cout << "Haupt-Orientierung: Std: " << main_angle << "°" << endl;

        //vector<unsigned int> values{main_angle, main_angle2};
        imshow("Main angles:" , showAngle(frame, main_angle, true));
        waitKey(0);
    }
}