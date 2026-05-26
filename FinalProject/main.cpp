// main.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This program accepts two images on the command line and 
//	outputs a third image with coloured bounding boxes drawn 
//	around every meaningful region that differs between them.
//
// Pipeline:
// Align images with ORB feature matching and homography
// Compute absolute difference of the two images
// Reduce noise with morphological operations
// Find contours of the remaining regions
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;

// main
// Preconditions: 
// Postconditions: 
int main()
{

}