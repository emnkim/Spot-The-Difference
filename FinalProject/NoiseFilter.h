/*
File: NoiseFilter.h
Author: Emily Kim and UV Raz
Description: The class takes the grayscale difference image and cleans it into a binary mask suitable for contour detection.
             Since absolute differencing picks up minor lighting variation and sensor noise.
             The four operations are applied in this order:

             1. Thresholding cuts out weak pixel differences
             2. Gaussian blur smooths jagged edges
             3. Morphological opening removes small isolated speckle noise
             4. Morphological closing fills holes within genuine changed regions

Assumptions: Input diff image is a grayscale Mat (CV_8UC1) produced by DifferenceDetector.
             Images being compared are mostly similar with only small localized changes.
             Input image is non-empty and has been aligned by ImageAligner before differencing.

References:
             https://pyimagesearch.com/2021/04/28/opencv-morphological-operations/
             https://docs.opencv.org/3.4/d3/dbe/tutorial_opening_closing_hats.html
*/
#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
    Purpose: Applies binary thresholding to the diff image to isolate regions.
             Any pixel below threshValue becomes 0 (no change).
             Any pixel at or above threshValue becomes 255 (change).
    Pre-conditions: diff is a non-empty CV_8UC1 grayscale Mat.
                    threshValue is between 0 and 255.
    Post-conditions: Returns a binary Mat the same size as diff.
                     Output contains only values {0, 255}.
*/
Mat applyThreshold(const Mat& diff, int threshValue = 30);


/*
    Purpose: Smooths the binary mask with a Gaussian kernel to soften jagged edges.
             Merge nearby blobs.
             Re-thresholds at 127 afterwards to keep the output binary.
    Pre-conditions: binary is a non-empty CV_8UC1 binary Mat.
                    kernelSize is a positive and odd integer.
    Post-conditions: Returns a binary Mat the same size as binary.
                     Output contains only values {0, 255}.
*/
Mat applyBlur(const Mat& binary, int kernelSize = 5);


/*
    Purpose: Applies morphological opening to remove small isolated noise specks that survived thresholding.
             Larger kernelSize or more iterations removes larger specks.
    Pre-conditions: binary is a non-empty CV_8UC1 binary Mat.
                    kernelSize is a positive and odd integer.
                    iterations is >= 1.
    Post-conditions: Returns a binary Mat the same size as binary.
                     Small isolated regions are removed
                     Large regions are preserved.
*/
Mat morphOpen(const Mat& binary, int kernelSize = 3, int iterations = 1);


/*
    Purpose: Applies morphological closing go fill small holes and gaps inside changed regions.
             Larger kernelSize fills bigger holes.
    Pre-conditions: binary is a non-empty CV_8UC1 binary Mat.
                    kernelSize is a positive and odd integer.
                    iterations is >= 1.
    Post-conditions: Returns a binary Mat the same size as binary.
                     Holes inside white regions are filled.
                     Overall shape is preserved.
*/
Mat morphClose(const Mat& binary, int kernelSize = 7, int iterations = 2);


/*
    Purpose: Runs the filtering pipeline on a raw diff image to produce a binary mask ready for contour detection.
             Applies in order: 
                         1. Thresholding: cuts out weak pixel differences
                         2. Gaussian blur: smooths jagged edges
                         3. Morphological opening: removes small isolated speckle noise
                         4. Morphological closing: fills holes within genuine changed regions
    Pre-conditions: diff is a non-empty CV_8UC1 grayscale Mat from DifferenceDetector.
                    threshValue is between 0 and 255.
                    blurKernel, openKernel, and closeKernel are positive and odd integers.
    Post-conditions: Returns a clean binary Mat the same size as diff.
                     Noise specks are removed and holes in real change regions are filled.
*/
Mat cleanMask(const Mat& diff, int threshValue = 30, int blurKernel  = 5, int openKernel  = 3, int closeKernel = 7);