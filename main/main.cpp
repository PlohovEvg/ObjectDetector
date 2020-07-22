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
		r = Rect(xTL, yTL, xBR - xTL, yTL - yBR);
		Iou = iou(r, chosen_box);
		flag = false;
		break;
	}
}

int main()
{ 
	int n = 5; //Set n value
	int max_score = 2; //Set max_score value
	float IOUthresh = 0.9f;
	float probThresh = 0.3f;
	float NMSThresh = 0.45f;
	int ran, boxran;
	unsigned chosen_class;
	std::string chosen_class_name;
	int score = 0;
	int counter = 0;
	std::vector<Mat> images;
	std::vector<Rect> boxes;
	std::vector<float> probs;
	std::vector<unsigned> classes;
	std::vector<std::string> classesNames;
	std::string line;
	std::ifstream ifst(join(DATA_FOLDER, "classes.txt"));
	Mat chosen_pic;
	Mat copy;
	Detector model;

	//Mat img = imread("C:\\openvino_practice\\data\\conference.png");
	Mat img = imread(join(DATA_FOLDER, "1.jpg"));
	imshow("before", img);
	waitKey();

	model.detect(img, NMSThresh, probThresh, boxes, probs, classes);

	for (size_t i = 0; i < boxes.size(); i++)
	{
		rectangle(img, boxes[i], Scalar(0, 255, 0));
	}
	imshow("after", img);
	waitKey();

	//while (std::getline(ifst, line))
	//{
	//	classesNames.push_back(line);
	//}

	//for (int i = 0; i < n; i++)
	//{
	//	images.push_back(imread(join(DATA_FOLDER, to_string(i + 1) + ".jpg")));
	//}
	//
	//srand(time(NULL));
	//
	//for (int i = 0; i < n; i++)
	//{
	//	if (score == max_score)
	//	{
	//		Mat A = Mat::zeros(200, 50, CV_8U);
	//		namedWindow("You win!", WINDOW_NORMAL);
	//		putText(A, "YOU WIN!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
	//		return 1;
	//	}
	//	
	//	Iou = -1.0f;
	//	boxes.clear();
	//	classes.clear();
	//	probs.clear();
	//	
	//	ran = rand() % (n + 1 - counter);
	//	counter++;	
	//	chosen_pic = images[ran];
	//	images.erase(images.begin() + (ran - 1));
	//	

	//	model.detect(chosen_pic, NMSThresh, probThresh, boxes, probs, classes);
	//	cout << "11" << endl;
	//	boxran = rand() % (int)(boxes.size());
	//	cout << "12" << endl;
	//	chosen_class = classes[boxran];
	//	cout << "13" << endl;
	//	chosen_class_name = classesNames[chosen_class];
	//	cout << "14" << endl;
	//	chosen_box = boxes[boxran];
	//	cout << "15" << endl;

	//	namedWindow("img", WINDOW_NORMAL);
	//	cout << "16" << endl;
	//	setMouseCallback("img", onMouse);
	//	cout << "17" << endl;

	//	//Ask user to highlight chosen object

	//	cout << "Please, highlight " << chosen_class_name << endl;

	//	while (true)
	//	{
	//		chosen_pic.copyTo(copy);
	//		rectangle(copy, r, Scalar(0, 0, 255));
	//		imshow("img", copy);
	//		char key = waitKey(30);

	//		if (Iou != -1.0f)
	//		{
	//			break;
	//		}						
	//	}

	//	if (Iou >= IOUthresh)
	//	{
	//		score++;
	//		cout << "current score:" << score << "  target score:" << max_score << endl;
	//	}
	//	else
	//	{
	//		Mat A = Mat::zeros(200, 50, CV_8U);
	//		namedWindow("Game over!", WINDOW_NORMAL);
	//		putText(A, "GAME OVER!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
	//		break;
	//	}
	//}
	return 0;
}