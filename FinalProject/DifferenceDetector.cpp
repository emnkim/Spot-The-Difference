// DifferenceDetector.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This file contains the implementation of the DifferenceDetector class,
//	which is responsible for computing the difference between two aligned images,
//	reducing noise, and finding contours of meaningful regions.
#include "DifferenceDetector.h"
#include "types.h"
#include "NoiseFilter.h"
#include <opencv2/imgproc.hpp>

// detectDifferences
// Preconditions: img1 and img2 are valid, aligned images.
// Postconditions: Returns a vector of contours representing meaningful regions of difference.
std::vector<std::vector<cv::Point>> DifferenceDetector::detectDifferences(const cv::Mat& img1, const cv::Mat& img2)
{
	return {};
}