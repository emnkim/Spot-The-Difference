// RegionHighlighter.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This file contains the implementation of the RegionHighlighter class,
//	which is responsible for highlighting regions of interest in an image.
#include "RegionHighlighter.h"
#include "types.h"
#include <opencv2/imgproc.hpp>

// highlightRegions
// Preconditions: img is a valid image, contours is a vector of contours representing regions of interest.
// Postconditions: Returns a copy of img with bounding boxes drawn around the contours.
cv::Mat RegionHighlighter::highlightRegions(const cv::Mat& img, const std::vector<std::vector<cv::Point>>& contours) 
{
	return {};
}