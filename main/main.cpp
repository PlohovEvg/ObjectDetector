#include "nms_and_iou.h"

using namespace cv;
int xTL, yTL, xBR, yBR;
bool flag = false;
Rect r;
Mat image;

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
	image = imread("D:\walking.jpg");
	Mat copy;
	namedWindow("img", WINDOW_NORMAL);
	setMouseCallback("img", onMouse);
	while (1)
	{
		image.copyTo(copy);
		rectangle(copy, r, Scalar(0, 0, 255));
		imshow("img", copy);
		char key = waitKey(30);
		if (key == 27)
		{
			break;
		}
	}
	return 0;
}