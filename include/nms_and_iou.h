#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace cv::utils::fs;

float iou(const cv::Rect& a, const cv::Rect& b);

void nms(const std::vector<cv::Rect>& boxes, const std::vector<float>& probabilities,
	float threshold, std::vector<unsigned>& indices);