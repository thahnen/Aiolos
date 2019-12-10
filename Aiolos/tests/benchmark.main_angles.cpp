//
// Created by Tobias Hahnen on 10.12.19.
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

    vector<GLCM::Method> meths = {
        GLCM::TOP_2,
        GLCM::TOP_3,
        GLCM::MEDIAN,
        GLCM::AVERAGE,
        GLCM::L_QUARTILE
    };

    for (const char* path : images) {
        Mat image = imread(path);
        if (image.channels() != 1) {
            cvtColor(image, image, COLOR_BGR2GRAY);
        }

        for (GLCM::Method meth : meths) {
            vector<unsigned int> angles;
            vector<unsigned int> times;

            /**
             *  GLCM::STANDARD, max_r := 50
             */
            for (int i = 0; i < 10; i++) {
                auto begin = chrono::steady_clock::now();
                angles = GLCM::main_angles(image, GLCM::STANDARD, meth, 50);
                unsigned int time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-begin).count();

                times.push_back(time);
            }

            cout << "Datei: " << path << endl
                 << "Methode: " << meth << endl
                 << "Dom. Winkel (Standard): " << endl;
            for (unsigned int angle : angles) cout << " " << angle << "°";
            cout << endl
                 << "Durch. Zeit (Standard): " << accumulate(times.begin(), times.end(), 0.0) / times.size() / 1000
                 << " Sekunden" << endl;


            /**
             * GLCM::SCHEME2, max_r := 50
             */
            angles.clear();
            times.clear();
            for (int i = 0; i < 10; i++) {
                auto begin = chrono::steady_clock::now();
                angles = GLCM::main_angles(image, GLCM::SCHEME2, meth, 50);
                unsigned int time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-begin).count();

                times.push_back(time);
            }

            cout << "Dom. Winkel (Scheme 2): " << endl;
            for (unsigned int angle : angles) cout << " " << angle << "°";
            cout << endl
                 << "Durch. Zeit (Scheme 2): " << accumulate(times.begin(), times.end(), 0.0) / times.size() / 1000
                 << " Sekunden" << endl << endl;
        }
    }

    return 0;
}
