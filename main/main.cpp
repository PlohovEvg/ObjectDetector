#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <detection.h>
using namespace cv;
using namespace std;

float maxIOU;
Rect r;
vector<Rect> chosen_boxes;


struct Region {
    int click;// on off
    int X1; //initial coordination based on EVENT_LBUTTONDOWN
    int Y1;
    int X2; // actual coordination 
    int Y2;
    int move;
}chosen;

static void onMouse(int event, int x, int y, int flags, void* img) {
    if (event == EVENT_RBUTTONDOWN) {
        chosen.click = 0;
        //cout << "right down " << endl;
        return;
    }
    if (event == EVENT_LBUTTONDOWN) {
        chosen.X1 = x;
        chosen.Y1 = y;
        chosen.click = 1;
        //cout << "left down" << endl;
        return;
    }
    if (event == EVENT_MOUSEMOVE) {
        if (chosen.click == 1) {
            chosen.X2 = x;
            chosen.Y2 = y;
            r = Rect(chosen.X1, chosen.Y1, chosen.X2 - chosen.X1, chosen.Y2 - chosen.Y1);
            //cout << "left up" << endl;
        }
        return;
    }
    if (event == EVENT_LBUTTONUP) {
        //cout << "move" << endl;
		float Iou;
        chosen.X2 = x;
        chosen.Y2 = y;		
        r = Rect(chosen.X1, chosen.Y1, chosen.X2 - chosen.X1, chosen.Y1 - chosen.Y2);
		for (size_t i = 0; i < chosen_boxes.size(); i++)
		{
			Iou = iou(chosen_boxes[i], r);
			if (Iou > maxIOU)
			{
				maxIOU = Iou;
			}
		}
        chosen.click = 0;
        return;
    }
}

int main() {
    const int n = 20; 
    const int max_score = 20; 
    float IOUthresh = 0.7f;
	int ranpic, ranbox;
    int score = 0;
    int counter = 0;
    vector<float> probs;
	vector<Rect> boxes;
	vector<unsigned> classes;
	vector<unsigned> pic_indexes; //from 1 to 20
    Mat chosen_pic;
    Mat copy;
    std::vector<std::string> className;
	string chosen_class_name;
	Detector det;

	
	for (int i = 0; i < n; i++)
	{
		pic_indexes.push_back(i + 1);
	}


	srand(time(NULL));

    for (int i = 0; i < n; i++) {
		probs.clear();  //clear vectors with each iteration
		boxes.clear();
		classes.clear();
		className.clear();
		chosen_boxes.clear();

		//Select random picture from folder /data/
		ranpic = rand() % pic_indexes.size(); //randomize index of vector "pic_indexes" to select image

		chosen_pic = imread(join(DATA_FOLDER, to_string(pic_indexes[ranpic]) + ".jpg")); 

		pic_indexes.erase(pic_indexes.begin() + ranpic); //remove used index of picture

        det.detect(chosen_pic, 0.45f, 0.3f, boxes, probs, classes, className);

		ranbox = rand() % boxes.size(); //randomize index to select random object
		chosen_boxes.push_back(boxes[ranbox]); //push chosen_box
		chosen_class_name = className[ranbox];

		//search for objects with the same class_name
		for (int i = 0; i < boxes.size(); i++)
		{
			if ((i != ranbox) && (className[i] == chosen_class_name))
			{
				chosen_boxes.push_back(boxes[i]);  
			}
		}

        std::string text = "Find " + chosen_class_name;  //we should use putText here
        


        if (score == max_score) {
            destroyWindow(text);
            Mat A = imread(join(DATA_FOLDER, "0.jpg"));
            putText(A, "YOU WIN!", Point(180, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255),8,5);
            std::string result = "Score:" + std::to_string(score);
            putText(A, result, Point(180, 300), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 255), 4,3);
            imshow("WIN", A);
            waitKey();
            break;
        }
        maxIOU = -1.0f;                
        namedWindow(text, WINDOW_AUTOSIZE);
        setMouseCallback(text, onMouse);

        while (1) {
            chosen_pic.copyTo(copy);
            rectangle(copy, r, Scalar(0, 0, 255));
            imshow(text, copy);
            char key = waitKey(30);
            /*if (key == 27)break;*/

            if (maxIOU != -1.0f) {
                break;
            }
        }
        destroyWindow(text);

        if (maxIOU >= IOUthresh) {
            destroyWindow(text);

            score++;
            cout << "current score:" << score << "  target score:" << max_score << endl;
            Mat A = imread(join(DATA_FOLDER, "0.jpg"));
            putText(A, "RIGHT!", Point(180, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255),8,5);
            std::string result = "Score:" + std::to_string(score);
            putText(A, result, Point(180, 300), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 255), 4,3);
            imshow("WIN", A);
            waitKey();
        }
        else {
            destroyWindow(text);

            Mat A = imread(join(DATA_FOLDER, "0.jpg"));
            putText(A, "GAME OVER!", Point(180, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255),8,5);
            std::string result = "Score:" + std::to_string(score);
            putText(A, result, Point(180, 300), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 255), 4,3);
            imshow("GAME OVER :(", A);
            waitKey();
            break;
        }
    }
    return 0;
}