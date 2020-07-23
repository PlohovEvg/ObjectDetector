#include "../include/detector.hpp"
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <inference_engine.hpp>
#include <fstream>

using namespace cv;
using namespace InferenceEngine;


Detector::Detector() {

}


Blob::Ptr Detector::wrapMat2Blob(const Mat& m) {
    CV_Assert(m.depth() == CV_8U);
    std::vector<size_t> dims = { 1, (size_t)m.channels(), (size_t)m.rows, (size_t)m.cols };
    return make_shared_blob<uint8_t>(TensorDesc(Precision::U8, dims, Layout::NHWC), m.data);
}

void Detector::detect(const cv::Mat& image,
    float nmsThreshold,
    float probThreshold,
    std::vector<cv::Rect>& boxes,
    std::vector<float>& probabilities,
    std::vector<unsigned>& classes) {
    Core ie;
    //Load deep learning network into memory
    CNNNetwork network = ie.ReadNetwork(utils::fs::join(DATA_FOLDER, "retinanet-tf.xml"),
        utils::fs::join(DATA_FOLDER, "retinanet-tf.bin"));

    InputInfo::Ptr input_info = network.getInputsInfo().begin()->second;
    std::string input_name = network.getInputsInfo().begin()->first;
    input_info->getPreProcess().setResizeAlgorithm(RESIZE_BILINEAR);
    input_info->setLayout(Layout::NHWC);
    input_info->setPrecision(Precision::U8);

    DataPtr output_info = network.getOutputsInfo().begin()->second;
    std::string output_name = network.getOutputsInfo().begin()->first;
    output_info->setPrecision(Precision::FP32);
    ExecutableNetwork exec_net = ie.LoadNetwork(network, "CPU");
    InferRequest req = exec_net.CreateInferRequest();
    Blob::Ptr input = wrapMat2Blob(image); 
    req.SetBlob(input_name, input); 
    req.Infer(); 
    float* output = req.GetBlob(output_name)->buffer(); 
    int size = req.GetBlob(output_name)->size() / 7; 
    
    for (int i = 0; i < size; i++) {
        int indx = i * 7; //std::cout << "56\n";
        float Probability = output[indx + 2];// std::cout << "57\n";
        if (Probability > probThreshold) {
            int HEIGHT = image.rows;
            int WIDTH = image.cols;
            int xmin = output[indx + 3] * WIDTH;
            int ymin = output[indx + 4] * HEIGHT;
            int xmax = output[indx + 5] * WIDTH;
            int ymax = output[indx + 6] * HEIGHT;
            Rect rectangle(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
            boxes.push_back(rectangle);
            probabilities.push_back(Probability);
            classes.push_back(output[indx + 1]);
        }
    }
    
    std::vector<unsigned> indices;
    nms(boxes, probabilities, nmsThreshold, indices); 
    int Size = boxes.size();
    int k = 0, j = 0;

    /*
    for (size_t i = 0; i < indices.size(); i++)
    {
        boxes.push_back(boxes[indices[i]]);
        probabilities.push_back(probabilities[indices[i]]);
        classes.push_back(classes[indices[i]]);
    }*/
    std::vector<std::string> className;
    std::ifstream ifs(utils::fs::join(DATA_FOLDER, "classes.txt"));
    std::string line;
    while (std::getline(ifs, line)) {
        className.push_back(line);
    }
    cv::Mat img_copy = image.clone();
    Size = boxes.size();
    for (int i = 0; i < Size; i++) {
        cv::rectangle(img_copy, boxes[i], cv::Scalar(255, 0, 0), 2);
        cv::putText(img_copy, cv::String(std::to_string(probabilities[i])+className[i]), cv::Point(boxes[i].x, boxes[i].y), 3, 1, cv::Scalar(0, 0, 255)/*, 0.5, 8, false*/);
        std::cout << "ClassName = "<<className[i]<<"\tProb = "<<probabilities[i] <<"\tBoxesCoord = ["<<boxes[i].x<<","<<boxes[i].y<<"]\t height = "<<boxes[i].height<<"\tweight = "<<boxes[i].width<<"\n";

    }
    
    imshow("image_result", img_copy);
    waitKey();
}





void nms(const std::vector<cv::Rect>& boxes, const std::vector<float>& probabilities,
    float threshold, std::vector<unsigned>& indices)
{
    std::vector<int> mark(boxes.size() + 1, 0);
    std::vector<int>::iterator it;
    int ind;
    float IOU;
    float maxProb;

    mark[boxes.size()] = -1;

    it = std::find(mark.begin(), mark.end(), 0);

    while (it != mark.end())
    {
        maxProb = 0.0f;

        for (size_t i = 0; i < probabilities.size(); i++)
        {
            if (mark[i] == 0)
            {
                if (probabilities[i] > maxProb)
                {
                    maxProb = probabilities[i];
                    ind = i;
                }
            }
        }

        indices.push_back(ind);
        mark[ind] = 1;

        for (size_t i = 0; i < probabilities.size(); i++)
        {
            if (mark[i] == 0)
            {
                IOU = iou(boxes[ind], boxes[i]);

                if (IOU >= threshold)
                {
                    mark[i] = 1;
                }
            }
        }

        it = std::find(mark.begin(), mark.end(), 0);
    }
}

float iou(const cv::Rect& a, const cv::Rect& b) {
    float s;
    s = (a & b).area();
    float SU = a.area() + b.area() - s;
    return s / SU;
}

