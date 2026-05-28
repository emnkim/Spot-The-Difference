// RegionHighlighter.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This file contains the declaration of the RegionHighlighter class,
//	which is responsible for highlighting regions of interest in an image.
#pragma once
#include <vector>
#include <opencv2/core.hpp>

class RegionHighlighter {
public:
	// highlightRegions
	// Preconditions: img is a valid image, contours is a vector of contours representing regions of interest.
	// Postconditions: Returns a copy of img with bounding boxes drawn around the contours.
	cv::Mat highlightRegions(const cv::Mat& img, const std::vector<std::vector<cv::Point>>& contours);
};