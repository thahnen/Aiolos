//
// Created by thahnen on 30.08.19.
//

#include <iostream>
#include <numeric>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <GLCM.h>

using namespace std;
using namespace cv;


int main() {
    vector<const char*> images = {
#ifdef WIN32
        "../../../../assets/test_images/sar_plane.png",
        "../../../../assets/test_images/sar_strip.png",
        "../../../../assets/test_images/sar_tracks.jpg",
        "../../../../assets/test_images/sea_400x400.jpg",
        "../../../../assets/test_images/maps_texel_sea.png",
        "../../../../assets/test_images/zebrastreifen.jpg"
#else
        "../../assets/test_images/sar_plane.png",
        "../../assets/test_images/sar_strip.png",
        "../../assets/test_images/sar_tracks.jpg",
        "../../assets/test_images/sea_400x400.jpg",
        "../../assets/test_images/maps_texel_sea.png",
        "../../assets/test_images/zebrastreifen.jpg"
#endif
    };

    for (const char* path : images) {
        Mat image = imread(path);
        if (image.channels() != 1) {
            cvtColor(image, image, COLOR_BGR2GRAY);
        }

        unsigned int angle;
        vector<unsigned int> times;

        /**
         *  GLCM::STANDARD, max_r := 50
         */
        for (int i = 0; i < 10; i++) {
            auto begin = chrono::steady_clock::now();
            angle = GLCM::main_angle(image, GLCM::STANDARD, 50);
            unsigned int time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-begin).count();

            times.push_back(time);
        }

        cout << "Datei: " << path << endl
            << "Dom. Winkel (Standard): " << angle << "°" << endl
            << "Durch. Zeit (Standard): " << accumulate(times.begin(), times.end(), 0.0) / times.size() / 1000
            << " Sekunden" << endl;


        /**
         * GLCM::SCHEME2, max_r := 50
         */
        times.clear();
        for (int i = 0; i < 10; i++) {
            auto begin = chrono::steady_clock::now();
            angle = GLCM::main_angle(image, GLCM::SCHEME2, 50);
            unsigned int time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-begin).count();

            times.push_back(time);
        }

        cout << "Dom. Winkel (Scheme 2): " << angle << "°" << endl
             << "Durch. Zeit (Scheme 2): " << accumulate(times.begin(), times.end(), 0.0) / times.size() / 1000
             << " Sekunden" << endl << endl;
    }

    return 0;
}