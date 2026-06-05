// types.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
//
// Implementation file for global configuration variables.
#include "types.h"

// Image alignment parameters
int minMatchCount = 10;          // Minimum number of ORB matches to consider a valid homography
double diffThreshold = 80.0;     // Pixel intensity difference threshold for identifying meaningful regions

// Noise filtering parameters
int gaussKernelSize = 7;          // Size of the Gaussian blur kernel for noise reduction
int morphOpenKernelSize = 5;          // Size of the morphological open operation kernel for noise reduction
int morphCloseKernelSize = 11;          // Size of the morphological close operation kernel for noise reduction

// Contour filtering parameters
double minContourArea = 5000.0;    // Minimum area of contours to be considered meaningful regions

// Visualization parameters
cv::Scalar boxColor = cv::Scalar(0, 255, 0); // Color of the bounding boxes (green)