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

#include "Evaluation.h"
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;


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
EvalMetrics evaluate(const Mat& predicted, const Mat& groundTruth) {
    Mat pred, gt;
    threshold(predicted, pred, 127, 1, THRESH_BINARY);
    threshold(groundTruth, gt, 127, 1, THRESH_BINARY);
    pred.convertTo(pred, CV_32S);
    gt.convertTo(gt,   CV_32S);

    Mat tp_mat, fp_mat, fn_mat;
    bitwise_and(pred, gt, tp_mat);
    subtract(pred, gt, fp_mat); 
    fp_mat = max(fp_mat, 0);
    subtract(gt, pred, fn_mat); 
    fn_mat = max(fn_mat, 0);

    double tp = sum(tp_mat)[0];
    double fp = sum(fp_mat)[0];
    double fn = sum(fn_mat)[0];

    EvalMetrics m;
    m.precision = (tp + fp > 0) ? tp / (tp + fp) : 0.0;
    m.recall = (tp + fn > 0) ? tp / (tp + fn) : 0.0;
    m.f1 = (m.precision + m.recall > 0) ? 2.0 * m.precision * m.recall / (m.precision + m.recall) : 0.0;
    m.iou = (tp + fp + fn > 0) ? tp / (tp + fp + fn) : 0.0;
    return m;
}


/*
    Purpose: Prints a summary of an Metrics struct to stdout.
             Used per image pair during batch evaluation over the LEVIR-CD dataset.
    Pre-conditions: metrics is a valid Metrics struct from evaluate().
                    label is an optional string to identify the image pair.
    Post-conditions: Precision, recall, F1, and IoU are printed to stdout.
                     No return value.
*/
void printMetrics(const EvalMetrics& m, const string& label) {
    cout << fixed << setprecision(4);
    if (!label.empty()) cout << "[" << label << "]\n";
    cout << "  Precision : " << m.precision << "\n"
         << "  Recall : " << m.recall << "\n"
         << "  F1 : " << m.f1 << "\n"
         << "  IoU : " << m.iou << "\n";
}