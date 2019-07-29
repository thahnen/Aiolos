# Aiolos : Implementaion of a GLCM-Based Texture Orientation Estimation Method

## Abstract

Implementation of the Texture Orientation Estimation Method described in this [Paper](Paper.pdf) by the [IEEE](https://ieeexplore.ieee.org/abstract/document/8331276).
Uses a [GLCM](https://de.wikipedia.org/wiki/Grauwertematrix) to find the main orientation(s) in a given image.

### Usability

Right now only as a C++ implementation, not as a (shared) library to use yet!
Also there are two different schemes as described in the paper implemented by now (Standard and Scheme 2).
Both implementations are also available in two versions, one for use at compile time (CT) and one for run time (RT).

### TODO's

Take a look at the Issues-Page!
Always looking for some optimization or making the "library" more usable!
Add support for multiple main orientations in one single image!

---

## Prerequisites

Needs following software installed:
1. Some kind of **Unix**-based OS => there is no Windows implementation yet!
2. [CMake](https://cmake.org/) to build the project
2. [OpenCV 4+](https://opencv.org/) for working on images
3. [OpenMP](https://www.openmp.org/) for parallelization

---

## Results

I am sorry there are none to show yet! I will add some when I am back at the office!

---

## Name?

Named after the [ancient greek god of winds](https://en.wikipedia.org/wiki/Aeolus_(Odyssey)) just because the paper used the method for Sea Surface Wind Direction Retrieval from SAR Imagery!
