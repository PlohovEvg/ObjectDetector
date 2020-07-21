#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <inference_engine.hpp>

using namespace cv;
using namespace std;
using namespace cv::utils::fs;
using namespace InferenceEngine;


float iou(const Rect& a, const Rect& b);

void nms(const vector<Rect>& boxes, const vector<float>& probabilities,
	float threshold, vector<unsigned>& indices);