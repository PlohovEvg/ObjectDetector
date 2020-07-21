#include "detection.h"


int xTL, yTL, xBR, yBR;
bool flag = false;
float Iou;
Rect r;
Rect chosen_box;

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
		Iou = iou(r, chosen_box);
		flag = false;
		break;
	}
}

int main()
{ 
	int n; //Set n value
	int max_score = 10; //Set max_score value
	float IOUthresh = 0.9f;
	float probThresh = 0.3f;
	float NMSThresh = 0.45f;
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
	Detector model;

	for (int i = 0; i < n; i++)
	{
		images[i] = imread(join(DATA_FOLDER, to_string(i + 1) + ".jpg"));
	}

	srand(time(NULL));

	for (int i = 0; i < n; i++)
	{
		if (score == max_score)
		{
			Mat A = Mat::zeros(200, 50, CV_8U);
			namedWindow("You win!", WINDOW_NORMAL);
			putText(A, "YOU WIN!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
			break;
		}

		Iou = -1.0f;
		boxes.clear();
		classes.clear();
		probs.clear();

		ran = rand() % (n + 1 - counter);
		counter++;
		chosen_pic = images[ran];
		images.erase(images.begin() + (ran - 1));

		model.detect(chosen_pic, NMSThresh, probThresh, boxes, probs, classes);
		
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

			if (Iou != -1.0f)
			{
				break;
			}						
		}

		if (Iou >= IOUthresh)
		{
			score++;
			cout << "current score:" << score << "  target score:" << max_score << endl;
		}
		else
		{
			Mat A = Mat::zeros(200, 50, CV_8U);
			namedWindow("Game over!", WINDOW_NORMAL);
			putText(A, "GAME OVER!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
			break;
		}
	}
	return 0;
}