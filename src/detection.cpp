#include "detection.h"

Detector::Detector() {
	Core ie;

	// Load deep learning network into memory
	auto net = ie.ReadNetwork(utils::fs::join(DATA_FOLDER, "faster-rcnn-resnet101-coco-sparse-60-0001.xml"),
		utils::fs::join(DATA_FOLDER, "faster-rcnn-resnet101-coco-sparse-60-0001.bin"));
	InputInfo::Ptr inputInfo = net.getInputsInfo()["image"];

	inputInfo->getPreProcess().setResizeAlgorithm(ResizeAlgorithm::RESIZE_BILINEAR);
	inputInfo->setLayout(Layout::NHWC);
	inputInfo->setPrecision(Precision::U8);
	outputName = net.getOutputsInfo().begin()->first;

	ExecutableNetwork execNet = ie.LoadNetwork(net, "CPU");

	req = execNet.CreateInferRequest();
}

Blob::Ptr wrapMatToBlob(const Mat& m) {
	CV_Assert(m.depth() == CV_8U);
	std::vector<size_t> dims = { 1, (size_t)m.channels(), (size_t)m.rows, (size_t)m.cols };
	return make_shared_blob<uint8_t>(TensorDesc(Precision::U8, dims, Layout::NHWC),
		m.data);
}

void Detector::detect(const cv::Mat& image,
	float nmsThreshold,
	float probThreshold,
	std::vector<cv::Rect>& boxes,
	std::vector<float>& probabilities,
	std::vector<unsigned>& classes) {
	Blob::Ptr input = wrapMatToBlob(image);
	req.SetBlob("image", input);
	req.Infer();
	float *output = req.GetBlob(outputName)->buffer();
	size_t out_size = req.GetBlob(outputName)->size();
	size_t reckt_num = out_size / 7;

	cout << out_size << endl << endl;

	for (size_t i = 0; i < out_size; i+=7)
	{
		cout << output[i] << ", " << output[i + 1] << ", " << output[i + 2] << ", " << output[i + 3] << ", " << output[i + 4] << ", " << output[i + 5] << ", " << output[i + 6] << endl;
	}

	std::vector<cv::Rect> tempBoxes;
	std::vector<float> tempProbs;
	std::vector<unsigned> tempClasses;

	int WIDTH = image.cols;
	int HEIGHT = image.rows;

	for (size_t i = 0; i < reckt_num; i++)
	{
		if (output[7 * i + 2] >= probThreshold)
		{
			int x = (int)(output[7 * i + 3] * WIDTH);
			cout << "63" << endl;
			int y = (int)(output[7 * i + 4] * HEIGHT);
			cout << "64" << endl;
			int width = (int)(output[7 * i + 5] * WIDTH) - (int)(output[7 * i + 3] * WIDTH) + 1;
			cout << "65" << endl;
			int height = (int)(output[7 * i + 6] * HEIGHT) - (int)(output[7 * i + 4] * HEIGHT) + 1;
			cout << "66" << endl;

			tempClasses.push_back((unsigned)output[7 * i + 1]);
			cout << "67" << endl;
			tempProbs.push_back(output[7 * i + 2]);
			cout << "68" << endl;
			tempBoxes.push_back(Rect(x, y, width, height));
			cout << "69" << endl;
		}
	}
	std::vector<unsigned> inds;
	nms(tempBoxes, tempProbs, nmsThreshold, inds);
	cout << "70" << endl;

	for (size_t i = 0; i < inds.size(); i++)
	{
		boxes.push_back(tempBoxes[inds[i]]);
		cout << "71" << endl;
		probabilities.push_back(tempProbs[inds[i]]);
		cout << "72" << endl;
		classes.push_back(tempClasses[inds[i]]);
		cout << "73" << endl;
	}
}


float iou(const cv::Rect& a, const cv::Rect& b)
{
	return (float)(a & b).area() / (float)((a.area() + b.area() - (a & b).area()));
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