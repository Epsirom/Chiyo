#ifndef MEDIAN_H
#define MEDIAN_H

#include <opencv2/opencv.hpp>

cv::Mat apply_median(cv::Mat img, int window_radius = 1);

#endif // MEDIAN_H
