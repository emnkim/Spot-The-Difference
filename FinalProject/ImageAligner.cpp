// ImageAligner.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
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
#include <opencv2/highgui.hpp>

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

    // Normalize both images before feature detection
    cv::Mat img1Gray, img2Gray, img1Norm, img2Norm;
    cv::cvtColor(img1, img1Gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, img2Gray, cv::COLOR_BGR2GRAY);
    cv::normalize(img1Gray, img1Norm, 0, 255, cv::NORM_MINMAX);
    cv::normalize(img2Gray, img2Norm, 0, 255, cv::NORM_MINMAX);

	// Detect ORB features and compute descriptors.
	cv::Ptr<cv::ORB> orb = cv::ORB::create(10000);
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	orb->detectAndCompute(img1Norm, cv::noArray(), keypoints1, descriptors1);
	orb->detectAndCompute(img2Norm, cv::noArray(), keypoints2, descriptors2);

	// Check if we have enough keypoints
	if (keypoints1.size() < minMatchCount || keypoints2.size() < minMatchCount) {
		std::cerr << "Not enough keypoints detected in one or both images." << std::endl;
		return img2;
	}

	// Match features using the Brute-Force matcher.
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> matches;
    matcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<cv::DMatch> goodMatches;
    for (const auto& match : matches) {
        if (match.size() == 2 && match[0].distance < 0.95f * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }

	std::cout << "Total matches: " << matches.size() << ", Good matches: " << goodMatches.size() << std::endl;

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

    cv::Mat affine = cv::estimateAffinePartial2D(points2, points1, cv::noArray(), cv::RANSAC, 5.0);

    if (affine.empty()) {
        std::cerr << "Count not compute affine transform." << std::endl;
        return img2;
    }

    cv::Mat alignedImg;
    cv::warpAffine(img2, alignedImg, affine, img1.size());

    cv::imwrite("debug_aligned.png", alignedImg);
    return alignedImg;
}