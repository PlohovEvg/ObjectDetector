#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <nms_and_iou.h>
using namespace cv;
using namespace std;

float Iou;
Rect chosen_box;


struct Region {
    int click;// on off
    int X1; //initial coordination based on EVENT_LBUTTONDOWN
    int Y1;
    int X2; // actual coordination 
    int Y2;
    int move;
}chosen;

static void CallBackF(int event, int x, int y, int flags, void* img) {
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
    if (event == EVENT_LBUTTONUP) {
        chosen.X2 = x;
        chosen.Y2 = y;
        chosen.click = 2;
        chosen.move = 0;
        //cout << "left up" << endl;
        return;
    }
    if (event == EVENT_MOUSEMOVE) {
        //cout << "move" << endl;
        chosen.X2 = x;
        chosen.Y2 = y;
        chosen.move++;
        return;
    }
}

int main(){
    //int n; //Set n value
    //int max_score; //Set max_score value
    chosen.click = 0;
    chosen.move = 0;

    float IOUthresh = 0.9f;
    int ran, boxran;
    unsigned chosen_class;
    int score = 0;
    int counter = 0;
    //std::vector<Mat> images(n);
    std::vector<Rect> boxes;
    std::vector<float> probs;
    std::vector<unsigned> classes;
    Mat chosen_pic;
    Mat copy;
    std::vector<Mat> images;
    string s;
    int k = 1;
    
	//srand(time(NULL));

	for (int i = 0; i < 5; i++){
		if (score == 5){
			Mat A = Mat::zeros(200, 50, CV_8U);
			namedWindow("You win!", WINDOW_NORMAL);
			putText(A, "YOU WIN!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
			break;
		}

		//Iou = -1.0f;
		//boxes.clear();
		//classes.clear();
		//probs.clear();

		//ran = i;
		//chosen_pic = images[ran];
		//images.erase(images.begin() + i);

		////Detect objects on chosen_pic

		//boxran = rand() % boxes.size();
		//chosen_class = classes[boxran];
		//chosen_box = boxes[boxran];
        s = to_string(k);
        Mat img;
        img = imread(join(DATA_FOLDER, s + ".jpg"));
		//Ask user to highlight chosen object
        k++;
        while (1) {
            img = imread(join(DATA_FOLDER, s + ".jpg"));
            Mat im = img.clone();
            namedWindow("image", WINDOW_AUTOSIZE);
            setMouseCallback("image", CallBackF, 0);

            if (chosen.click == 1) {
                rectangle(img, Rect(chosen.X1, chosen.Y1, chosen.X2 - chosen.X1, chosen.Y2 - chosen.Y1), Scalar(0, 0, 255), 2, 15);
            }
            else if (chosen.click == 2 && chosen.move == 1) {
                Rect rect(chosen.X1, chosen.Y1, chosen.X2 - chosen.X1, chosen.Y2 - chosen.Y1);
                boxes.push_back(rect);
            }

            imshow("image", img);
            const int key = cv::waitKey(27);
            if (key == 27)break;
        }

		if (Iou >= IOUthresh){
			score++;
			cout << "current score:" << score << "  target score:" << 5 << endl;
		}
		else{
			Mat A = Mat::zeros(200, 50, CV_8U);
			namedWindow("Game over!", WINDOW_NORMAL);
			putText(A, "GAME OVER!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
			break;
		}
	}
	return 0;
}