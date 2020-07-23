#include "detection.h"
Detector::Detector() {
    Core ie;

    auto network = ie.ReadNetwork(join(DATA_FOLDER, "retinanet-tf.xml"),
            join(DATA_FOLDER, "retinanet-tf.bin"));

    InputInfo::Ptr input_info = network.getInputsInfo()["input_1"];

    input_info->getPreProcess().setResizeAlgorithm(RESIZE_BILINEAR);
    input_info->setLayout(Layout::NHWC);
    input_info->setPrecision(Precision::U8);
    outputName = network.getOutputsInfo().begin()->first;

    ExecutableNetwork exec_net = ie.LoadNetwork(network, "CPU");

    req = exec_net.CreateInferRequest();
}


Blob::Ptr wrapMat2Blob(const Mat& m) {
    CV_Assert(m.depth() == CV_8U);
    vector<size_t> dims = { 1, (size_t)m.channels(), (size_t)m.rows, (size_t)m.cols };
    return make_shared_blob<uint8_t>(TensorDesc(Precision::U8, dims, Layout::NHWC), m.data);
}

void Detector::detect(const cv::Mat& image,
    float nmsThreshold,
    float probThreshold,
    vector<Rect>& boxes,
    vector<float>& probabilities,
    vector<unsigned>& classes) {
    Blob::Ptr input = wrapMat2Blob(image);

    req.SetBlob("input_1", input);
    req.Infer();

    float* output = req.GetBlob(outputName)->buffer();
    int size = req.GetBlob(outputName)->size() / 7;

    for (int i = 0; i < size; i++) {
        int indx = i * 7;
        float Probability = output[indx + 2];
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
}





void nms(const vector<Rect>& boxes, const vector<float>& probabilities,
    float threshold, vector<unsigned>& indices)
{
    vector<int> mark(boxes.size() + 1, 0);
    vector<int>::iterator it;
    int ind;
    float IOU;
    float maxProb;

    mark[boxes.size()] = -1;

    it = find(mark.begin(), mark.end(), 0);

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

        for (size_t i = 0; i < probabilities.size(); i++)        {
            if (mark[i] == 0)            {
                IOU = iou(boxes[ind], boxes[i]);

                if (IOU >= threshold)                {
                    mark[i] = 1;
                }
            }
        }

        it = find(mark.begin(), mark.end(), 0);
    }
}

float iou(const Rect& a, const Rect& b) {
    float s;
    s = (a & b).area();
    float SU = a.area() + b.area() - s;
    return s / SU;
}