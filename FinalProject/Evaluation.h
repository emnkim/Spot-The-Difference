/*
File: Evaluation.h
Author: Emily Kim and UV Raz
Description: This module measures how accurately the pipeline detects real changes
             by comparing the predicted binary mask against the ground truth labels. 
             It computes four standard metrics: precision (how many detections were
             actually correct), recall (how many real changes were successfully found),
             F1 (the balance between the two), and IoU (the ratio of overlap to total
             area covered).

Assumptions: Predicted and ground truth masks are both CV_8UC1 binary Mats.
             Both masks are the same size.
             Ground truth labels are provided by the LEVIR-CD dataset label folder.
             White pixels (255) represent changed regions, black (0) represent unchanged.
*/

#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;


struct Metrics {
    double precision;   // TP / (TP + FP)
    double recall;      // TP / (TP + FN)
    double f1;          // 2 * (precision * recall) / (precision + recall)
    double iou;         // TP / (TP + FP + FN)
};


/*
    Purpose: Compares the predicted binary mask to ground truth 
             mask pixel-by-pixel to compute precision, recall, F1, and IoU.
    Pre-conditions: predicted is a CV_8UC1 binary Mat output from NoiseFilter.
                    groundTruth is a CV_8UC1 binary Mat from the LEVIR-CD label folder.
                    Both Mats must be the same size.
    Post-conditions: Returns an Metrics struct with all four metrics computed.
                     Values are between 0.0 and 1.0.
                     Returns zeros for any metric where the denominator is zero.
*/
Metrics evaluate(const Mat& predicted, const Mat& groundTruth);


/*
    Purpose: Prints a summary of an Metrics struct to stdout.
             Used per image pair during batch evaluation over the LEVIR-CD dataset.
    Pre-conditions: metrics is a valid Metrics struct from evaluate().
                    label is an optional string to identify the image pair.
    Post-conditions: Precision, recall, F1, and IoU are printed to stdout.
                     No return value.
*/
void printMetrics(const Metrics& metrics, const string& label = "");