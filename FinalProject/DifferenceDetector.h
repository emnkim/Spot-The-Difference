// DifferenceDetector.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
//
// This file contains the declaration of the DifferenceDetector class,
//	which is responsible for computing the difference between two aligned images,
//	reducing noise, and finding contours of meaningful regions.
#pragma once
#include <vector>
#include <opencv2/core.hpp>

class DifferenceDetector {
public:
	// detectDifferences
	// Preconditions: img1 and img2 are valid, aligned images.
	// Postconditions: Returns a vector of contours representing meaningful regions of difference.
	std::vector<std::vector<cv::Point>> detectDifferences(const cv::Mat& img1, const cv::Mat& img2);

private:
	// convertToGrayscale
	// Preconditions: img is a valid image.
	// Postconditions: Returns a grayscale version of img.
	//                 If img is already grayscale, returns a copy of img.
	cv::Mat convertToGrayscale(const cv::Mat& img);

	// computeDifference
	// Preconditions: gray1 and gray2 are valid grayscale images of the same size.
	// Postconditions: Returns the absolute difference between gray1 and gray2.
	cv::Mat computeDifference(const cv::Mat& gray1, const cv::Mat& gray2);
};