/*
File: NoiseFilter.cpp
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

#include "NoiseFilter.h"
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
Mat applyThreshold(const Mat& diff, int threshValue) {
    Mat output;
    threshold(diff, output, threshValue, 255, THRESH_BINARY);

    return output;
}

/*
    Purpose: Smooths the binary mask with a Gaussian kernel to soften jagged edges.
             Merge nearby blobs.
             Re-thresholds at 127 afterwards to keep the output binary.
    Pre-conditions: binary is a non-empty CV_8UC1 binary Mat.
                    kernelSize is a positive and odd integer.
    Post-conditions: Returns a binary Mat the same size as binary.
                     Output contains only values {0, 255}.
*/
Mat applyBlur(const Mat& binary, int kernelSize) {
    Mat blurred, output;
    GaussianBlur(binary, blurred, Size(kernelSize, kernelSize), 0);
    threshold(blurred, output, 127, 255, THRESH_BINARY);

    return output;
}

/*
    Purpose: Applies morphological opening to remove small isolated noise specks that survived thresholding.
             Larger kernelSize or more iterations removes larger specks.
    Pre-conditions: binary is a non-empty CV_8UC1 binary Mat.
                    kernelSize is a positive and odd integer.
                    iterations is >= 1.
    Post-conditions: Returns a binary Mat the same size as binary.
                     Small isolated regions are removed
                     Large regions are preserved.
*/          Small isolated regions are removed; large regions are preserved.
*/
Mat morphOpen(const Mat& binary, int kernelSize, int iterations) {
    Mat eroded, output;
    Mat kernelEllip = getStructuringElement(MORPH_ELLIPSE, Size(kernelSize, kernelSize));
    erode(binary, eroded, kernelEllip, Point(-1, -1), iterations);
    dilate(eroded, output, kernelEllip, Point(-1, -1), iterations);

    return output;
}

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
Mat morphClose(const Mat& binary, int kernelSize, int iterations) {
    Mat dilated, output;
    Mat kernelEllip = getStructuringElement(MORPH_ELLIPSE, Size(kernelSize, kernelSize));
    dilate(binary, dilated, kernelEllip, Point(-1, -1), iterations);
    erode(dilated, output, kernelEllip, Point(-1, -1), iterations);

    return output;
}

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
Mat cleanMask(const Mat& diff, int threshValue, int blurKernel, int openKernel, int closeKernel) {
    Mat mask;
    mask = applyThreshold(diff, threshValue);
    mask = applyBlur(mask, blurKernel);
    mask = morphOpen(mask, openKernel);
    mask = morphClose(mask, closeKernel);

    return mask;
}