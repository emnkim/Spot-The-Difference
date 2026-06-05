// types.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
//
// Central home for configuration and type definitions for the image differencing program.
#pragma once
#include <opencv2/core.hpp>

// Image alignment parameters
extern int minMatchCount;      // Minimum number of ORB matches to consider a valid homography
extern double diffThreshold;   // Pixel intensity difference threshold for identifying meaningful regions

// Noise filtering parameters
extern int gaussKernelSize;     // Size of the Gaussian blur kernel for noise reduction
extern int morphOpenKernelSize;     // Size of the morphological open operation kernel for noise reduction
extern int morphCloseKernelSize;     // Size of the morphological close operation kernel for noise reduction

// Contour filtering parameters
extern double minContourArea;   // Minimum area of contours to be considered meaningful regions

// Visualization parameters
extern cv::Scalar boxColor;     // Color of the bounding boxes (green)