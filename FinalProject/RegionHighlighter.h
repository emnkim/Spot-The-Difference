/*
File: RegionHighlighter.h
Author: Emily Kim and UV Raz
Description: This module takes the binary mask from NoiseFilter and identifies
             the specific regions where change has occurred. 
             It uses contour detection to find connected groups of white pixels, filters out anything too small,
             and wraps each surviving region in a bounding box drawn directly onto the original image. 
             The output is an image that clearly marks every detected change, along with a list
             of region data (position, size, area).

Assumptions: Input mask is a binary CV_8UC1 Mat produced by NoiseFilter.
             Input original image is a non-empty BGR Mat the same size as the mask.
             minArea of 200.0 pixels filters out small noise contours.
             Bounding boxes are drawn in red (0, 0, 255) by default.

References:
             https://learnopencv.com/contour-detection-using-opencv-python-c/
             https://stackoverflow.com/questions/56183201/detect-and-visualize-differences-between-two-images-with-opencv-python
*/

#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;


struct DetectedRegion {
    int x, y; // top-left corner of bounding box
    int w, h; // width and height
    double area; // contour area in pixels
    vector<Point> contour; // raw contour points
};


/*
    Purpose: Finds contours in the binary mask and returns all regions
             whose area is >= minArea.
    Pre-conditions: mask is a non-empty CV_8UC1 binary Mat.
                    minArea is a positive value.
    Post-conditions: Returns a vector of DetectedRegion structs, one per changed region. 
                     Returns empty vector if no regions meet the minArea threshold.
*/
vector<DetectedRegion> findRegions(const Mat& mask, double minArea = 200.0);


/*
    Purpose: Draws a colored bounding box around each DetectedRegion onto a copy of the image. 
             Optionally draws a numbered label above each box.
    Pre-conditions: image is a non-empty BGR or grayscale Mat.
                    regions is a vector of DetectedRegion from findRegions.
    Post-conditions: Returns a BGR Mat the same size as image with bounding boxes drawn.
                     Input image is not modified.
*/
Mat drawBoundingBoxes(const Mat& image, const vector<DetectedRegion>& regions, Scalar color = Scalar(0, 0, 255),
                      int thickness = 2, bool label = true);


/*
    Purpose: Runs the full detection pipeline by calling findRegions then 
             drawBoundingBoxes in one step.
    Pre-conditions: mask is a non-empty CV_8UC1 binary Mat from NoiseFilter.
                    originalImg is the same size as mask.
                    minArea is a positive value.
    Post-conditions: regions is populated with all detected changed regions.
                     Returns a BGR annotated Mat the same size as originalImg.
*/
Mat detectAndDraw(const Mat& mask, const Mat& originalImg, vector<DetectedRegion>& regions, double minArea = 200.0);