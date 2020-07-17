#include "nms_and_iou.h"

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