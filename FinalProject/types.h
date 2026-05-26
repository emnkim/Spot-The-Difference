// types.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// Central home for configuration and type definitions for the image differencing program.
#include <opencv2/core.hpp>

int minMatchCount = 10; // Minimum number of ORB matches to consider a valid homography
double threshold = 30.0; // Pixel intensity difference threshold for identifying meaningful regions
int gaussKernelSize = 5; // Size of the Gaussian blur kernel for noise reduction
int morphKernelSize = 3; // Size of the morphological operation kernel for noise reduction
double minContourArea = 100.0; // Minimum area of contours to be considered meaningful regions
cv::Scalar boxColor = cv::Scalar(0, 255, 0); // Color of the bounding boxes (green)