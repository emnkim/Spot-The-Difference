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
#include <iostream>
#include <opencv2/highgui.hpp>


// convertToGrayscale
// Preconditions: img is a valid image.
// Postconditions: Returns a grayscale version of img.
//                 If img is already grayscale, returns a copy of img.
cv::Mat DifferenceDetector::convertToGrayscale(const cv::Mat& img) {
	cv::Mat gray;

	// Check if image is already grayscale
	if (img.channels() == 1) {
		gray = img.clone();
	}
	// Convert BGR to grayscale
	else if (img.channels() == 3) {
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	}
	// Convert BGRA to grayscale
	else if (img.channels() == 4) {
		cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
	}
	else {
		std::cerr << "Unsupported number of channels: " << img.channels() << std::endl;
		gray = img.clone();
	}

	return gray;
}

// computeDifference
// Preconditions: gray1 and gray2 are valid grayscale images of the same size.
// Postconditions: Returns the absolute difference between gray1 and gray2.
cv::Mat DifferenceDetector::computeDifference(const cv::Mat& gray1, const cv::Mat& gray2) {
	cv::Mat diff;

	// Validate inputs
	if (gray1.size() != gray2.size()) {
		std::cerr << "Error: Images must be the same size for differencing." << std::endl;
		return cv::Mat();
	}

	if (gray1.channels() != 1 || gray2.channels() != 1) {
		std::cerr << "Error: Both images must be grayscale for differencing." << std::endl;
		return cv::Mat();
	}

	// Compute absolute difference
	cv::absdiff(gray1, gray2, diff);

	return diff;
}

// detectDifferences
// Preconditions: img1 and img2 are valid, aligned images.
// Postconditions: Returns a vector of contours representing meaningful regions of difference.
// detectDifferences
// Preconditions: img1 and img2 are valid, aligned images.
// Postconditions: Returns a vector of contours representing meaningful regions of difference.
std::vector<std::vector<cv::Point>> DifferenceDetector::detectDifferences(const cv::Mat& img1, const cv::Mat& img2)
{
    // Validate inputs
    if (img1.empty() || img2.empty()) {
        std::cerr << "Error: One or both input images are empty." << std::endl;
        return {};
    }

    // Convert both images to grayscale
    cv::Mat gray1 = convertToGrayscale(img1);
    cv::Mat gray2 = convertToGrayscale(img2);

    // Compute absolute difference
    cv::Mat diff = computeDifference(gray1, gray2);
    cv::imwrite("1demo_diff.png", diff);

    if (diff.empty()) {
        std::cerr << "Error: Failed to compute difference." << std::endl;
        return {};
    }

    // Noise filtering
    //cv::Mat mask = cleanMask(diff, 80, 7, 5, 11);
    //cv::imwrite("mask.png", mask);


    // DEMO
    int border = 30;
    cv::rectangle(diff, cv::Point(0, 0), cv::Point(diff.cols-1, diff.rows-1), cv::Scalar(0), border);
    cv::imwrite("2demo_diff_bordered.png", diff);
    cv::Mat step1 = applyThreshold(diff, 80);
    cv::imwrite("3demo_threshold.png", step1);
    cv::Mat step2 = applyBlur(step1, 7);
    cv::imwrite("4demo_blur.png", step2);
    cv::Mat step3 = morphOpen(step2, 5);
    cv::imwrite("5demo_morph_open.png", step3);
    cv::Mat mask = morphClose(step3, 11);
    cv::imwrite("6debug_final_mask.png", mask);

    // Contour detection
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter out small contours
    std::vector<std::vector<cv::Point>> filteredContours;
    for (const auto& contour : contours) {
        if (cv::contourArea(contour) >= minContourArea) {
            filteredContours.push_back(contour);
        }
    }

    return filteredContours;
}