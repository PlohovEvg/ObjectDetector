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
Rect r;
Rect chosen_box;


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
        chosen.X2 = x;
        chosen.Y2 = y;
        r = Rect(chosen.X1, chosen.Y1, chosen.X2 - chosen.X1, chosen.Y2 - chosen.Y1);
        Iou = iou(r, chosen_box);
        chosen.click = 0;
        return;
    }
}

int main() {
    const int n = 1; //Set n value
    const int max_score = 5; //Set max_score value
    float IOUthresh = 0.9f;
    int ran, boxran;
    unsigned chosen_class;
    int score = 0;
    int counter = 0;
    std::vector<Mat> images;
    std::vector<Rect> boxes;
    std::vector<float> probs;
    std::vector<unsigned> classes;
    Mat chosen_pic;
    Mat copy;

    for (int i = 0; i < 1; i++) {
        images.push_back(imread(join(DATA_FOLDER, to_string(i + 1) + ".jpg")));
    }
    //srand(time(NULL));
    for (int i = 0; i < 1; i++) {
        //if (score == max_score) {
        //    Mat A = Mat::zeros(200, 50, CV_8U);
        //    namedWindow("You win!", WINDOW_NORMAL);
        //    putText(A, "YOU WIN!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
        //    break;
        //}
        //Iou = -1.0f;
        //boxes.clear();
        //classes.clear();
        //probs.clear();

        //ran = rand() % (n + 1 - counter);
        //counter++;
        //chosen_pic = images[ran];
        //images.erase(images.begin() + (ran - 1));

        ////Detect objects on chosen_pic

        //boxran = rand() % boxes.size();
        //chosen_class = classes[boxran];
        //chosen_box = boxes[boxran];

        chosen_pic = images[i];
        namedWindow("img", WINDOW_NORMAL);
        setMouseCallback("img", onMouse);
        while (1) {
            chosen_pic.copyTo(copy);
            rectangle(copy, r, Scalar(0, 0, 255));
            imshow("img", copy);
            char key = waitKey(30);
            if (key == 27)break;

            /*if (Iou != -1.0f) {
                break;
            }*/
        }
        /*if (Iou >= IOUthresh) {
            score++;
            cout << "current score:" << score << "  target score:" << max_score << endl;
        }
        else {
            Mat A = Mat::zeros(200, 50, CV_8U);
            namedWindow("Game over!", WINDOW_NORMAL);
            putText(A, "GAME OVER!", Point(10, 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));
            break;
        }*/
    }
    return 0;
}