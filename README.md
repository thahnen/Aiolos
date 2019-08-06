# Aiolos : Implementaion of a GLCM-Based Texture Orientation Estimation Method

## Abstract

Implementation of the Texture Orientation Estimation Method described in this [Paper](Paper.pdf) by the [IEEE](https://ieeexplore.ieee.org/abstract/document/8331276).
Uses a [GLCM](https://de.wikipedia.org/wiki/Grauwertematrix) to find the main orientation(s) in a given image.

### Usability

Implemented as a C++14 shared library!
Also there are two different schemes as described in the paper implemented by now (Standard and Scheme 2).
Both implementations are also available in two versions, one for use at compile time (CT) and one for run time (RT).

---

## Prerequisites

Needs following software installed:
1. Some kind of **Unix**-based OS => there is no Windows implementation yet!
2. [CMake](https://cmake.org/) to build the project
2. [OpenCV 4+](https://opencv.org/) for working on images
3. [OpenMP](https://www.openmp.org/) for parallelization

---

## Functions
TODO: which functions can be used from outside!

---

## Results

I am sorry there are none to show yet! I will add some when I am back at the office!

---

## TODO's

Take a look at the Issues-Page!
Always looking for some optimization or making the "library" more usable!

---

## Name?

Named after the [ancient greek god of winds](https://en.wikipedia.org/wiki/Aeolus_(Odyssey)) Because the paper used this method for Sea Surface Wind Direction Retrieval from SAR Imagery!
