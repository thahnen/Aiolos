#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


/**************************************************************************************************
 *
 *      HELPER FUNCTIONS
 *
 **************************************************************************************************/
unsigned int max_gray_value(Mat image) {
    switch (image.type() & CV_MAT_DEPTH_MASK) {
        case CV_8S:
            // 2^4 hoechster Wert
            return 16;
        case CV_8U:
        case CV_16S:
            // 2^8 hoechster Wert
            return 256;
        case CV_16U:
        case CV_32S:
            // 2^16 hoechster Wert
            return 65536;
        default:
            // Float etc nicht bedacht, erst einmal das zurückgeben!
            return -1;
    }
}


/**************************************************************************************************
 *
 *      ABSTRACT CLASS TO MAKE EVERY SCHEME ACCESSABLE
 *
 **************************************************************************************************/
class Scheme {
public:
    // Function to calculate the dominant texture orientation
    unsigned int min_theta(Mat image) {
        // Assertion: only one channel!
        assert(image.channels() == 1);

        vector<unsigned int> values;

        // Assertion: only values 0 <= x < 360
        for (int theta = 0; theta < 360; ++theta) {
            values.push_back(Z_(image, theta));
        }

        unsigned int min_value = *min_element(values.begin(), values.end());

        return arg(min_value);
    }

    virtual ~Scheme() = default;

protected:
    // GLCM matrix element (m,n) using relative position (r,theta)
    // => extended by Scheme 1+2!
    // => changed only by Scheme 3!
    unsigned int G_(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) {
        unsigned int q = Q_(image, r, theta);
        unsigned int value = 0;

        for (int i = 0; i < image.cols; ++i) {
            for (int j = 0; j < image.rows; ++j) {
                unsigned int x = i + r*cos(theta);
                unsigned int y = j + r*sin(theta);

                // Possible cast from smaller to higher value at "image.at<unsigned int>(...)" maybe not 100% efficient
                if (x >= 0 && x < image.cols && y >= 0 && y < image.cols
                    && image.at<unsigned int>(i, j) == m-1 && image.at<unsigned int>(x, y) == n-1) {
                    value++;
                }
            }
        }

        // return GLCM-Matrix element
        return value;
    }

    // Normalization factor
    // => changed only for Scheme 3!
    unsigned int Q_(Mat image, double r, unsigned int theta) {
        int value = 0;
        for (int i = 0; i < image.cols; ++i) {
            for (int j = 0; j < image.rows; ++j) {
                unsigned x = i + r*cos(theta);
                unsigned y = j + r*sin(theta);

                if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                    value++;
                }
            }
        }
    }

    // increasing function of the spacing between matrix element (m,n) and the diagonal line
    unsigned int w(unsigned int m, unsigned int n) {
        return pow(m-n, 2);
    }

    // degree of concentration of larger GLCM elements
    unsigned int Z(Mat image, double r, unsigned int theta) {
        unsigned int value = 0;
        unsigned int depth = max_gray_value(image);

        // Assertion: Matrix-Element (n,m) in Bounds!
        for (int i = 1; i < depth; ++i) {
            for (int j = 1; j < depth; ++j) {
                value += w(i, j) * G(image, i, j, r, theta);
            }
        }

        return value;
    }

    // same as abof but for practical usage!
    unsigned int Z_(Mat image, unsigned int theta) {
        unsigned int value = 0;
        unsigned int max_r = ceil(max(image.rows, image.cols) * sqrt(2)); // max radius to get the corners

        // Assertion: 1 <= r < max_r
        for (int r = 1; r < max_r; ++r) {
            value += Z(image, r, theta);
        }

        return value;
    }

    // Abstract method to be implemented by the different schemes
    virtual unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) = 0;
};


/**************************************************************************************************
 *
 *      SCHEME 1 IMPLEMENTATION
 *
 **************************************************************************************************/
class Scheme1 : public Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) {
        return (floor(r) + 1 - r) * G_(image, m, n, r, 0) + (r - floor(r)) * G_(image, m, n, r, 0);
    };
};


/**************************************************************************************************
 *
 *      SCHEME 2 IMPLEMENTATION
 *      => should be done!
 *
 **************************************************************************************************/
class Scheme2 : public Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) {
        double c_1 = (floor(r*cos(theta)) + 1 - r*cos(theta)) * (floor(r*sin(theta)) + 1 - r*sin(theta));
        double c_2 = (r*cos(theta) - floor(r*cos(theta))) * (floor(r*sin(theta)) + 1 - r*sin(theta));
        double c_3 = (floor(r*cos(theta)) + 1 - r*cos(theta)) * (r*sin(theta) - floor(r*sin(theta)));
        double c_4 = (r*cos(theta) - floor(r*cos(theta))) * (r*sin(theta) - floor(r*sin(theta)));

        unsigned int g_1 = G_(image, m, n, floor(r*cos(theta)), floor(r*sin(theta)));
        unsigned int g_2 = G_(image, m, n, floor(r*cos(theta)) + 1, floor(r*sin(theta)));
        unsigned int g_3 = G_(image, m, n, floor(r*cos(theta)), floor(r*sin(theta)) + 1);
        unsigned int g_4 = G_(image, m, n, floor(r*cos(theta)) + 1, floor(r*sin(theta)) + 1);

        return c_1*g_1 + c_2*g_2 + c_3*g_3 + c_4*g_4;
    }
};


/**************************************************************************************************
 *
 *      SCHEME 3 IMPLEMENTATION
 *
 **************************************************************************************************/
class Scheme3 : public Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    [[deprecated("Not implemented yet!")]]
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) {
        return -1;
    }

    // Normalization factor
    [[deprecated("Not implemented yet!")]]
    unsigned int Q(Mat image, double r, unsigned int theta) {
        return -1;
    }
};



/**************************************************************************************************
 *
 *      MAIN FUNCTION FOR TESTING
 *      TODO: does run infinitly?
 *
 **************************************************************************************************/
int main() {
    Mat image = imread("/home/thahnen/Downloads/zebrastreifen.jpg");
    imshow("Zebrastrifen:", image);

    Mat gray_image = image.clone();
    cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
    imshow("Zebrastreifen (Grayscale):", gray_image);
    waitKey(0);

    Scheme2 scheme;
    cout << "Winkel suchen ..." << endl;
    cout << "Winkel gefunden: " << scheme.min_theta(gray_image) << endl;

    waitKey(0);
    return 0;
}
