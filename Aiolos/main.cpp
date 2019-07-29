#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "GLCM.h"

using namespace std;
using namespace cv;


/**************************************************************************************************
 *
 *      MAIN FUNCTIONS FOR TESTING
 *
 **************************************************************************************************/
int main1() {
    /** Vergleich der Compile- + Runtime-Time Implementierung */

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
    unsigned int main_angle = GLCM::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle_ct = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2_ct = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
        << " Sec -> " << main_angle2_ct << "°" << endl;


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
    main_angle = GLCM::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle_ct = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;


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
    main_angle = GLCM::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle_ct = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;


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
    main_angle = GLCM::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle_ct = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;


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
    main_angle = GLCM::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle_ct = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle_ct << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2 << "°" << endl;

    begin = chrono::steady_clock::now();
    main_angle2_ct = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count()
         << " Sec -> " << main_angle2_ct << "°" << endl;

    return 0;
}



int main2() {
    Mat image = imread("../../test_images/zebrastreifen.jpg");
    if (!image.data) {
        cout << "Image not found or could not be loaded!" << endl;
        return 1;
    }

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
    cout << "Graustufenbild mit höchstem Grauwert: " << max_gray_value(gray_image) << endl;

    // Haupt-Orientierung erhalten & Schemata vergleichen!
    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    // Vegleich der beiden Möglichkeiten
    cout << "Haupt-Orientierung: Std: " << main_angle << "°, Sc2: " << main_angle2 << "°" << endl;


    showAngle(image, main_angle, true);
    showAngle(image, main_angle2, true);
}


int main() {
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
    cout << "Graustufenbild mit höchstem Grauwert: " << max_gray_value(gray_image) << endl;

    // Haupt-Orientierung erhalten & Schemata vergleichen!
    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::CT::main_angle(gray_image, STANDARD, 50);
    cout << "Std-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle2 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (RT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

#define SCHEME2_TEST true
    begin = chrono::steady_clock::now();
    unsigned int main_angle3 = GLCM::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (RT) v2: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
    unsigned int main_angle4 = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT): " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    begin = chrono::steady_clock::now();
#define SCHEME2_TEST_CT true
    unsigned int main_angle5 = GLCM::CT::main_angle(gray_image, SCHEME2, 50);
    cout << "Sc2-Dauer (CT) v2: " << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;

    // Vegleich der beiden Möglichkeiten
    cout << "Haupt-Orientierung: Std (CT): " << main_angle << "°, Sc2 (RT): " << main_angle2 << "°, Sc2 (CT): " << main_angle4 << "°" << endl;


    showAngle(image, main_angle, true);
}