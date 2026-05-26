// NoiseFilter.h
// by UV Raz and Emily Kim, 5/24/2026 - 6/__/2026
//
// This file contains the declaration of the NoiseFilter class,
//	which is responsible for reducing noise in the difference image using
//	Gaussian blur and morphological operations.

class NoiseFilter {
public:
	// reduceNoise
	// Preconditions: diffImg is a valid grayscale image representing the difference between two aligned images.
	// Postconditions: Returns a noise-reduced version of diffImg using Gaussian blur and morphological operations.
	cv::Mat reduceNoise(const cv::Mat& diffImg);
};