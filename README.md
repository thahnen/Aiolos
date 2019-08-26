# Aiolos : Implementaion of a GLCM-Based Texture Orientation Estimation Method

## Abstract

Implementation of the Texture Orientation Estimation Method described in this [Paper](Paper.pdf) by the [IEEE](https://ieeexplore.ieee.org/abstract/document/8331276).
Uses a [GLCM](https://de.wikipedia.org/wiki/Grauwertematrix) to find the main orientation(s) in a given image.

### Usability

Implemented as a C++14 shared library!
Also there are two different schemes as described in the paper implemented by now (Standard and Scheme 2).

---

## Prerequisites

Needs following software installed:
1. Some kind of **Unix**-based OS => Windows implementation not tested yet!
2. [CMake](https://cmake.org/) to build the project
2. [OpenCV 4+](https://opencv.org/) for working on images
3. [OpenMP](https://www.openmp.org/) for parallelization

---

## Functions

To get a single dominant angle of a given image one of these functions can be used.
The second one can be used to restrict the range of angles which should be considered in the computation as it is faster but the result may differs.
> There are different implementations of the GLCM calculation to choose from!

```cpp
unsigned int GLCM::main_angle(const cv::Mat& image, GLCM::Implementation impl, unsigned int max_r = 0);
unsigned int GLCM::main_angle(const cv::Mat& image, GLCM::Implementaion impl, const GLCM::Range& range, unsigned int max_r = 0);
```

To get multiple (or one) dominant angles of a given image one of these functions should be used!
The second one can be used to restrict the range of angles which should be considered in the computation as it is faster but the results may differ.
> There are different implementations of the GLCM calculation to choose from!
> There are different methods of calculating the dominant angles to choose from!

```cpp
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth, unsigned int max_r = 0);
std::vector<unsigned int> GLCM::main_angles(const cv::Mat& image, GLCM::Implementation impl, GLCM::Method meth, const GLCM::Range& range, unsigned int max_r = 0);
```

---

## Results

I am sorry there are none to show yet! I will add some when I am back at the office!

---

## TODO's

Take a look at the Issues-Page!
Always looking for some optimization or making the "library" more usable!

Add support for subimage, more different methods!

---

## Name?

Named after the [ancient greek god of winds](https://en.wikipedia.org/wiki/Aeolus_(Odyssey)) Because the paper used this method for Sea Surface Wind Direction Retrieval from SAR Imagery!
