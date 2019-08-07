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
int main() {
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
    unsigned int main_angle_ct = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2_ct = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle2_ct << "°" << endl;

    std::vector<unsigned int> values{main_angle, main_angle_ct, main_angle2, main_angle2_ct};
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
    main_angle_ct = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;

    std::vector<unsigned int> values2{main_angle, main_angle_ct, main_angle2, main_angle2_ct};
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
    main_angle_ct = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;

    std::vector<unsigned int> values3{main_angle, main_angle_ct, main_angle2, main_angle2_ct};
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
    main_angle_ct = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;

    std::vector<unsigned int> values4{main_angle, main_angle_ct, main_angle2, main_angle2_ct};
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
    main_angle_ct = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;

    std::vector<unsigned int> values5{main_angle, main_angle_ct, main_angle2, main_angle2_ct};
    showAngles(image, values5, true);
    waitKey(0);

    return 0;
}


/***********************************************************************************************************************
 *
 *      MAIN 2) TESTING DIFFERENT IMPLEMENTATIONS OF STANDARD + SCHEME 2 (OPTIMIZING)
 *
 ***********************************************************************************************************************/
int main2() {
    //Mat image = imread("../../test_images/maps_texel_sea.png");
    //Mat image = imread("../../test_images/sar_highway.jpg");
    //Mat image = imread("../../test_images/sar_landebahn.png");
    //Mat image = imread("../../test_images/sea_400x400.jpg");
    Mat image = imread("../../test_images/zebrastreifen.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

    // Haupt-Orientierung erhalten & Schemata vergleichen!
    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::CT::main_angle(gray_image, GLCM::STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (RT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

#define SCHEME2_TEST true
    begin = chrono::steady_clock::now();
    unsigned int main_angle3 = GLCM::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (RT) v2: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle4 = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
#define SCHEME2_TEST_CT true
    unsigned int main_angle5 = GLCM::CT::main_angle(gray_image, GLCM::SCHEME2, 50);
    cout << "Sc2-Dauer (CT) v2: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    // Vegleich der beiden Möglichkeiten
    cout << "Haupt-Orientierung: Std (CT): " << main_angle << "°, Sc2 (RT): " << main_angle2 << "°, Sc2 (CT): " << main_angle4 << "°" << endl;


    showAngle(image, main_angle, true);
    waitKey(0);
}


/***********************************************************************************************************************
 *
 *      MAIN 3) TESTING ON ACTUAL DATA (VIDEO)
 *      TODO: da kommt bisher irgendwie nur Mumpitz raus!
 *
 ***********************************************************************************************************************/
int main3() {
    //VideoCapture cap("../../test_images/c.1W.avi");
    VideoCapture cap("../../test_images/c.2R.avi");
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

        if (frame.channels() == 1) cvtColor(frame, frame, COLOR_BGR2GRAY);

        unsigned int main_angle = GLCM::CT::main_angle_range_(frame, GLCM::STANDARD, GLCM::Range(10, 90), 50);
        unsigned int main_angle2 = GLCM::CT::main_angle_range_(frame, GLCM::SCHEME2, GLCM::Range(10, 90), 50);

        cout << "Haupt-Orientierung: Std (CT): " << main_angle << "°, Sc2 (CT): " << main_angle2 << "°" << endl;

        vector<unsigned int> values{main_angle, main_angle2};
        showAngles(frame, values, true);
        waitKey(0);
    }
}