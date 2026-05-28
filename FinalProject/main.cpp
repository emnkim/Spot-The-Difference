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
#include "ImageAligner.h"
#include "DifferenceDetector.h"
#include "RegionHighlighter.h"

using namespace cv;

// main
// Preconditions: Command line should have 3 arguments: program name, input image 1, input image 2
// Postconditions: Creates and saves an output image with differences highlighted
int main(int argc, char** argv)
{
	// Check command line arguments
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <image1> <image2>" << std::endl;
		std::cerr << "Example: " << argv[0] << " photo1.jpg photo2.jpg" << std::endl;
		return -1;
	}

	// Load images
	Mat img1 = imread(argv[1]);
	Mat img2 = imread(argv[2]);

	if (img1.empty()) {
		std::cerr << "Error: Could not load image: " << argv[1] << std::endl;
		return -1;
	}

	if (img2.empty()) {
		std::cerr << "Error: Could not load image: " << argv[2] << std::endl;
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

	// Highlight the differences
	RegionHighlighter highlighter;
	Mat outputImg = highlighter.highlightRegions(img1, contours);

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