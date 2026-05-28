// ImageAligner.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This file contains the implementation of the ImageAligner class, 
//	which is responsible for aligning two images using ORB feature 
//	matching and homography estimation.
#include "ImageAligner.h"
#include "types.h"
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// align
// Preconditions: img1 and img2 are valid non-empty images.
// Postconditions: Returns the aligned version of img2, aligned to img1.
//                 If alignment fails, returns img2 unchanged.
cv::Mat ImageAligner::align(const cv::Mat& img1, const cv::Mat& img2) {
	// Validate input images
	if (img1.empty() || img2.empty()) {
		std::cerr << "Error: One or both input images are empty." << std::endl;
		return img2;
	}

	// Detect ORB features and compute descriptors.
	cv::Ptr<cv::ORB> orb = cv::ORB::create();
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	orb->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	orb->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	// Check if we have enough keypoints
	if (keypoints1.size() < minMatchCount || keypoints2.size() < minMatchCount) {
		std::cerr << "Not enough keypoints detected in one or both images." << std::endl;
		return img2;
	}

	// Match features using the Brute-Force matcher.
	cv::BFMatcher matcher(cv::NORM_HAMMING);
	std::vector<cv::DMatch> matches;
	matcher.match(descriptors1, descriptors2, matches);

	// Filter matches based on distance.
	// Calculate mean distance for adaptive thresholding
	double meanDistance = 0.0;
	for (const auto& match : matches) {
		meanDistance += match.distance;
	}
	meanDistance /= matches.size();

	// Use the threshold from types.h (default 30.0)
	double matchThreshold = std::min(diffThreshold, meanDistance * 0.7);

	std::vector<cv::DMatch> goodMatches;
	for (const auto& match : matches) {
		if (match.distance < matchThreshold) {
			goodMatches.push_back(match);
		}
	}

	std::cout << "Total matches: " << matches.size() 
			  << ", Good matches: " << goodMatches.size() << std::endl;

	// Check if we have enough good matches to compute homography.
	if (goodMatches.size() < minMatchCount) {
		std::cerr << "Not enough good matches to compute homography. Found: " 
				  << goodMatches.size() << ", Required: " << minMatchCount << std::endl;
		return img2; // Return the original image if alignment fails
	}

	// Extract location of good matches and compute homography.
	std::vector<cv::Point2f> points1, points2;
	for (const auto& match : goodMatches) {
		points1.push_back(keypoints1[match.queryIdx].pt);
		points2.push_back(keypoints2[match.trainIdx].pt);
	}

	// Use RANSAC to compute robust homography
	cv::Mat homography = cv::findHomography(points2, points1, cv::RANSAC, 5.0);

	// Check if homography computation was successful
	if (homography.empty()) {
		std::cerr << "Failed to compute homography matrix." << std::endl;
		return img2;
	}

	// Warp img2 to align with img1 using the computed homography.
	cv::Mat alignedImg;
	cv::warpPerspective(img2, alignedImg, homography, img1.size());

	return alignedImg;
}