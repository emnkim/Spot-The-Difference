// main.cpp
// by UV Raz and Emily Kim, 5/24/2026 - 6/04/2026
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
#include "ImageAligner.h"
#include "DifferenceDetector.h"
#include "RegionHighlighter.h"
#include "types.h"

using namespace cv;
using namespace std;

// main
// Preconditions: Command line should have 3 arguments: program name, input image 1, input image 2
//                OR run without arguments to use hardcoded test images
// Postconditions: Creates and saves an output image with differences highlighted
int main(int argc, char** argv)
{
	// Hardcoded image paths for testing
	string imagePath1 = "../../Before/test_1.png";
	string imagePath2 = "../../After/test_1.png";

	// If command line arguments are provided, use them instead
	if (argc == 3) {
		imagePath1 = argv[1];
		imagePath2 = argv[2];
		std::cout << "Using command line arguments..." << std::endl;
	}
	else {
		std::cout << "Using hardcoded test images..." << std::endl;
		std::cout << "  Image 1: " << imagePath1 << std::endl;
		std::cout << "  Image 2: " << imagePath2 << std::endl;
	}

	// Load images
	Mat img1 = imread(imagePath1);
	Mat img2 = imread(imagePath2);

	if (img1.empty()) {
		std::cerr << "Error: Could not load image: " << imagePath1 << std::endl;
		return -1;
	}

	if (img2.empty()) {
		std::cerr << "Error: Could not load image: " << imagePath2 << std::endl;
		return -1;
	}

	std::cout << "Loaded images successfully." << std::endl;
	std::cout << "Image 1 size: " << img1.size() << std::endl;
	std::cout << "Image 2 size: " << img2.size() << std::endl;

	// Align the images
	ImageAligner aligner;
	Mat alignedImg2 = aligner.align(img1, img2);

	// Detect differences
	DifferenceDetector detector;
	std::vector<std::vector<Point>> contours = detector.detectDifferences(img1, alignedImg2);

	// Convert contours to regions for highlighting
	std::vector<DetectedRegion> regions;
	for (const auto& contour : contours) {
		Rect bbox = boundingRect(contour);
		DetectedRegion r;
		r.x = bbox.x;
		r.y = bbox.y;
		r.w = bbox.width;
		r.h = bbox.height;
		r.area = contourArea(contour);
		r.contour = contour;
		regions.push_back(r);
	}

	// Highlight the differences
	Mat outputImg = drawBoundingBoxes(img1, regions, boxColor, 2, true);

	// Save the output
	std::string outputFilename = "differences_output.jpg";
	if (imwrite(outputFilename, outputImg)) {
		std::cout << "\nOutput saved to: " << outputFilename << std::endl;
	}
	else {
		std::cerr << "Error: Could not save output image." << std::endl;
		return -1;
	}

	// Display the images
	std::cout << "\nPress any key in the image windows to exit..." << std::endl;
	namedWindow("Original Image 1", WINDOW_AUTOSIZE);
	namedWindow("Aligned Image 2", WINDOW_AUTOSIZE);
	namedWindow("Differences Highlighted", WINDOW_AUTOSIZE);

	imshow("Original Image 1", img1);
	imshow("Aligned Image 2", alignedImg2);
	imshow("Differences Highlighted", outputImg);

	waitKey(0);

	return 0;
}