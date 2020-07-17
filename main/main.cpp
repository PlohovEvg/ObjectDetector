#include "nms_and_iou.h"

int xTL, yTL, xBR, yBR;
bool flag = false;
Rect r;

void onMouse(int event, int x, int y, int flags, void *userdata)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		xTL = x;
		yTL = y;
		flag = true;
		break;
	case EVENT_MOUSEMOVE:
		if (flag)
		{
			xBR = x;
			yBR = y;
			r = Rect(xTL, yTL, xBR - xTL, yBR - yTL);
		}
		break;
	case EVENT_LBUTTONUP:
		xBR = x;
		yBR = y;
		r = Rect(xTL, yTL, xBR - xTL, yBR - yTL);
		flag = false;
		break;
	}
}

int main()
{ 
	int n; //Set n value
	int max_score; //Set max_score value
	float IOUthresh = 0.9f;
	float Iou;
	int ran, boxran;
	unsigned chosen_class;
	int score = 0;
	int counter = 0;
	std::vector<Mat> images(n);
	std::vector<Rect> boxes;
	std::vector<float> probs;
	std::vector<unsigned> classes;
	Mat chosen_pic;
	Mat copy;
	Rect chosen_box;

	for (int i = 0; i < n; i++)
	{
		images[i] = imread(join(DATA_FOLDER, "*.jpg"));
	}

	srand(time(NULL));

	for (int i = 0; i < n; i++)
	{
		if (score == max_score)
		{
			Mat A = Mat::zeros(200, 50, CV_8U);
			namedWindow("img", WINDOW_NORMAL);
			putText(A, "YOU WIN!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 255, 0));
			break;
		}

		boxes.clear();
		classes.clear();
		probs.clear();

		ran = rand() % (n + 1 - counter);
		chosen_pic = images[ran];
		images.erase(images.begin() + (ran - 1));

		//Detect objects on chosen_pic
		
		boxran = rand() % boxes.size();
		chosen_class = classes[boxran];
		chosen_box = boxes[boxran];

		namedWindow("img", WINDOW_NORMAL);
		setMouseCallback("img", onMouse);

		//Ask user to highlight chosen object

		while (true)
		{
			chosen_pic.copyTo(copy);
			rectangle(copy, r, Scalar(0, 0, 255));
			imshow("img", copy);
			char key = waitKey(30);
			//Ask user to press enter
			if (key == 13)
			{
				break;
			}
		}

		Iou = iou(r, chosen_box);

		if (Iou >= IOUthresh)
		{
			score++;
		}
	}
	return 0;
}