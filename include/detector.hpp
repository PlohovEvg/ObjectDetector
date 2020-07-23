#pragma once
#include <opencv2/opencv.hpp>
#include <inference_engine.hpp>
using namespace cv;
using namespace InferenceEngine;
class Detector {
public:
    Detector();

    void detect(const cv::Mat& image,
        float nmsThreshold,
        float probThreshold,
        std::vector<cv::Rect>& boxes,
        std::vector<float>& probabilities,
        std::vector<unsigned>& classes);
    Blob::Ptr wrapMat2Blob(const Mat& m);
};

float iou(const cv::Rect& a, const cv::Rect& b);

void nms(const std::vector<cv::Rect>& boxes, const std::vector<float>& probabilities, float threshold, std::vector<unsigned>& indices);


