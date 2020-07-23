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

vector<float> Iou;
Rect r;
vector<Rect> chosen_box;


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
        for (int i = 0; i < chosen_box.size(); i++) {
            Iou.push_back(iou(r, chosen_box[i]));
        }
        chosen.click = 0;
        return;
    }
}

int main() {
    const int n = 20; //Set n value
    const int max_score = 20; //Set max_score value
    float IOUthresh = 0.7f;
    int ran, boxran;
    unsigned chosen_class;
    int score = 0;
    int counter = 0;
    std::vector<Mat> images;
    std::vector<float> probs;
    Mat chosen_pic;
    Mat copy;
    std::vector<std::string> className;


    for (int i = 0; i < n; i++) {
        images.push_back(imread(join(DATA_FOLDER, to_string(i + 1) + ".jpg")));
    }

    for (int i = 0; i < n; i++) {
        chosen.click = 0;
        r = Rect(0, 0, 0, 0);
        Detector det;
        cv::Mat img = images[i];
        std::vector<cv::Rect> boxes;
        std::vector<float> proboblilities;
        std::vector<unsigned> classes;
        std::cout << "Start Detector\n";
        det.detect(img, 0.8f, 0.7f, boxes, proboblilities, classes, className);

        string chose_name = className[classes[0]];
        std::string text = "Find " + chose_name;
        for (int i = 0; i < boxes.size(); i++) {
            if (className[i] == chose_name) {
                chosen_box.push_back(boxes[i]);
            }
        }

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
        Iou.clear();
        boxes.clear();
        classes.clear();
        probs.clear();
        className.clear();

        //ran = rand() % (n + 1 - counter);
        //counter++;
        //chosen_pic = images[ran];
        //images.erase(images.begin() + (ran - 1));

        ////Detect objects on chosen_pic

        //boxran = rand() % boxes.size();
        //chosen_class = classes[boxran];
        //chosen_box = boxes[boxran];

        chosen_pic = images[i];
        namedWindow(text, WINDOW_AUTOSIZE);
        setMouseCallback(text, onMouse);

        while (1) {
            chosen_pic.copyTo(copy);
            rectangle(copy, r, Scalar(0, 0, 255));
            imshow(text, copy);
            char key = waitKey(30);
            if (key == 27)break;

            if (Iou.size() != 0) {
                break;
            }
        }
        destroyWindow(text);
        int k = 0;
        for (int i = 0; i < Iou.size(); i++) {
            if (Iou[i] >= IOUthresh)k++;
        }
        if (k!=0) {
            destroyWindow(text);
            score++;
            cout << "current score:" << score << "  target score:" << max_score << endl;
            Mat A = imread(join(DATA_FOLDER, "0.jpg"));
            putText(A, "RIGHT!", Point(180, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255),8,5);
            std::string result = "Score:" + std::to_string(score);
            putText(A, result, Point(180, 300), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 255), 4,3);
            imshow("RIGHT", A);
            waitKey();
            destroyWindow("RIGHT");

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