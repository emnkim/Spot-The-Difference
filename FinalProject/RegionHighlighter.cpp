/*
File: RegionHighlighter.h
Author: Emily Kim and UV Raz
Description: This class takes the binary mask from NoiseFilter and identifies the specific regions where there are changes. 
             It uses contour detection to find connected groups of white pixels, filters out anything too small,
             and wraps the left over region in a bounding box drawn onto the original image. 
             The output is an image that marks every change. It will also create region data (position, size, area) 
             that can be used for further analysis or passed to evaluation for scoring with the ground truth of our testing dataset.

Assumptions: Input mask is a binary CV_8UC1 Mat produced by NoiseFilter.
             Input original image is same size as the mask.
             minArea of 200.0 pixels filters out small noise contours.
             Bounding boxes are drawn in red (0, 0, 255) by default.

References:
             https://learnopencv.com/contour-detection-using-opencv-python-c/
             https://stackoverflow.com/questions/56183201/detect-and-visualize-differences-between-two-images-with-opencv-python
*/

#include "RegionHighlighter.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
    Purpose: Finds contours in the binary mask and returns all regions
             whose area is >= minArea.
    Pre-conditions: mask is a non-empty CV_8UC1 binary Mat.
                    minArea is a positive value.
    Post-conditions: Returns a vector of DetectedRegion structs, one per changed region. 
                     Returns empty vector if no regions meet the minArea threshold.
*/
vector<DetectedRegion> findRegions(const Mat& mask, double minArea) {
    vector<vector<Point>> contours;
    findContours(mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<DetectedRegion> regions;
    for (const auto& cnt : contours) {
        double area = contourArea(cnt);
        if (area < minArea) {
            continue;
        }

        Rect bbox = boundingRect(cnt);
        DetectedRegion r;
        r.x = bbox.x;
        r.y = bbox.y;
        r.w = bbox.width;
        r.h = bbox.height;
        r.area = area;
        r.contour = cnt;
        regions.push_back(r);
    }
    return regions;
}


/*
    Purpose: Draws a colored bounding box around each DetectedRegion onto a copy of the image. 
             Optionally draws a numbered label above each box.
    Pre-conditions: image is a non-empty BGR or grayscale Mat.
                    regions is a vector of DetectedRegion from findRegions.
    Post-conditions: Returns a BGR Mat the same size as image with bounding boxes drawn.
                     Input image is not modified.
*/
Mat drawBoundingBoxes(const Mat& image, const vector<DetectedRegion>& regions, Scalar color, int thickness, bool label) {
    Mat output;
    if (image.channels() == 1){
        cvtColor(image, output, COLOR_GRAY2BGR);

    } else {
        output = image.clone();
    }

    for (int i = 0; i < (int)regions.size(); i++) {
        const DetectedRegion& r = regions[i];
        rectangle(output, Point(r.x, r.y), Point(r.x + r.w, r.y + r.h), color, thickness);
        if (label) {
            string text = "#" + to_string(i + 1) + " (" + to_string((int)r.area) + "px)";
            putText(output, text, Point(r.x, r.y - 6), FONT_HERSHEY_SIMPLEX, 0.45, color, 1, LINE_AA);
        }
    }
    return output;
}


/*
    Purpose: Runs the full detection pipeline by calling findRegions then 
             drawBoundingBoxes in one step.
    Pre-conditions: mask is a non-empty CV_8UC1 binary Mat from NoiseFilter.
                    originalImg is the same size as mask.
                    minArea is a positive value.
    Post-conditions: regions is populated with all detected changed regions.
                     Returns a BGR annotated Mat the same size as originalImg.
*/
Mat detectAndDraw(const Mat& mask, const Mat& originalImg, vector<DetectedRegion>& regions, double minArea) {
    regions = findRegions(mask, minArea);
    return drawBoundingBoxes(originalImg, regions);
}