// ImageAligner.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
//
// This file contains the declaration of the ImageAligner class, 
//	which is responsible for aligning two images using ORB feature 
//	matching and homography estimation.
#pragma once
#include <opencv2/core.hpp>

class ImageAligner {
public:
	// align
	// Preconditions: img1 and img2 are valid non-empty images.
	// Postconditions: Returns the aligned version of img2, aligned to img1.
	//                 If alignment fails, returns img2 unchanged.
	cv::Mat align(const cv::Mat& img1, const cv::Mat& img2);
};