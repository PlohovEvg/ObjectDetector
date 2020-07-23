#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <inference_engine.hpp>
#include <algorithm>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;
using namespace InferenceEngine;
using namespace cv::utils::fs;

class Detector {
public:
	Detector();

    void detect(const cv::Mat& image,
        float nmsThreshold,
        float probThreshold,
        std::vector<cv::Rect>& boxes,
        std::vector<float>& probabilities,
        std::vector<unsigned>& classes);  

private:
	InferenceEngine::InferRequest req;
	std::string outputName;
        
};

float iou(const cv::Rect& a, const cv::Rect& b);

void nms(const std::vector<cv::Rect>& boxes, const std::vector<float>& probabilities,
	float threshold, std::vector<unsigned>& indices);