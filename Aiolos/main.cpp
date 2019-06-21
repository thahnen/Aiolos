#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


/**************************************************************************************************
 *
 *      HELPING FUNCTIONS
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
        vector<unsigned int> values;

        for (int theta = 0; theta < 360; ++theta) {
            values.push_back(Z_(image, theta));
        }

        unsigned int min_value = *min_element(values.begin(), values.end());

        // Then use arg(min_value) but arg is for complex numbers?
    }

    virtual ~Scheme() = default;
private:
    // increasing function of the spacing between matrix element (m,n) and the diagonal line
    unsigned int w(unsigned int m, unsigned int n) {
        return pow(m-n, 2);
    }

    // degree of concentration of larger GLCM elements
    unsigned int Z(Mat image, double r, unsigned int theta) {
        unsigned int value = 0;
        unsigned int depth = max_gray_value(image);

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

        for (int r = 1; r < max_r; ++r) {
            value += Z(image, r, theta);
        }

        return value;
    }

    // Abstract methods to be implemented by the different schemes
    virtual unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta) = 0;
    virtual unsigned int Q(Mat image, double r, unsigned int theta) = 0;
};


/**************************************************************************************************
 *
 *      SCHEME 1 IMPLEMENTATION
 *
 **************************************************************************************************/
class Scheme1 : Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta);

    // Normalization factor
    unsigned int Q(Mat image, double r, unsigned int theta);
};


/**************************************************************************************************
 *
 *      SCHEME 2 IMPLEMENTATION
 *
 **************************************************************************************************/
class Scheme2 : Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta);

    // Normalization factor
    unsigned int Q(Mat image, double r, unsigned int theta);
};


/**************************************************************************************************
 *
 *      SCHEME 3 IMPLEMENTATION
 *
 **************************************************************************************************/
class Scheme3 : Scheme {
private:
    // GLCM matrix element (m,n) using relative position (r,theta)
    unsigned int G(Mat image, unsigned int m, unsigned int n, double r, unsigned int theta);

    // Normalization factor
    unsigned int Q(Mat image, double r, unsigned int theta);
};



/**************************************************************************************************
 *
 *      MAIN FUNCTION FOR TESTING
 *
 **************************************************************************************************/
int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
