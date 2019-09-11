//
// Created by thahnen on 30.08.19.
//

#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <GLCM.h>

using namespace std;
using namespace cv;


int main() {
#ifdef WIN32
	Mat image = imread("../../../../assets/test_images/sar_plane.png");
#else
    Mat image = imread("../../assets/test_images/sar_plane.png");
#endif
	if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    auto begin = chrono::steady_clock::now();
    unsigned int main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'sar_plane' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_plane' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'sar_plane' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_plane' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;


#ifdef WIN32
	image = imread("../../../../assets/test_images/sar_strip.png");
#else
    image = imread("../../assets/test_images/sar_strip.png");
#endif
	if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'sar_strip' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_strip' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'sar_strip' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_strip' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;


#ifdef WIN32
	image = imread("../../../../assets/test_images/sar_tracks.jpg");
#else
    image = imread("../../assets/test_images/sar_tracks.jpg");
#endif
    if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'sar_tracks' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_tracks' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'sar_tracks' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sar_tracks' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;


#ifdef WIN32
	image = imread("../../../../assets/test_images/sea_400x400.jpg");
#else
    image = imread("../../assets/test_images/sea_400x400.jpg");
#endif
	if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'sea_400x400' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sea_400x400' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'sea_400x400' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'sea_400x400' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;


#ifdef WIN32
	image = imread("../../../../assets/test_images/maps_texel_sea.png");
#else
    image = imread("../../assets/test_images/maps_texel_sea.png");
#endif
    if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'maps_texel_sea' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'maps_texel_sea' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'maps_texel_sea' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'maps_texel_sea' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;


#ifdef WIN32
	image = imread("../../../../assets/test_images/zebrastreifen.jpg");
#else
    image = imread("../../assets/test_images/zebrastreifen.jpg");
#endif
	if (image.channels() != 1) {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
    cout << "Dominanter Winkel 'zebrastreifen' (Standard) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'zebrastreifen' (Standard) - Winkel: " << main_angle << " Grad" << endl;

    begin = chrono::steady_clock::now();
    main_angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
    cout << "Dominanter Winkel 'zebrastreifen' (Scheme 2) - Dauer: "
         << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now()-begin).count() << " Sec" << endl;
    cout << "Dominanter Winkel 'zebrastreifen' (Scheme 2) - Winkel: " << main_angle << " Grad" << endl;
}