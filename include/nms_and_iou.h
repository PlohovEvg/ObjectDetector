#pragma once
#include <opencv2/opencv.hpp>

float iou(const cv::Rect& a, const cv::Rect& b);

void nms(const std::vector<cv::Rect>& boxes, const std::vector<float>& probabilities,
	float threshold, std::vector<unsigned>& indices);