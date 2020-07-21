#include "../include/detector.hpp"

#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <inference_engine.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>



int main(int argc, char **argv)
{
//	std::system("C:\\Program Files(x86)\\IntelSWTools\\openvino\\bin\\setupvars.bat");
	Detector det;
	cv::Mat img = cv::imread(cv::utils::fs::join(DATA_FOLDER, "product.jpg"), cv::IMREAD_COLOR);
	std::vector<cv::Rect> boxes;
	std::vector<float> proboblilities;
	std::vector<unsigned> classes;
	std::cout<<"Start Detector\n";
	det.detect(img, 0.1f, 0.1f, boxes, proboblilities, classes);
	std::cout << "complite\n";
	return 0;
}	