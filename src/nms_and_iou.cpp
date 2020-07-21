#include "nms_and_iou.h"



float iou(const Rect& a, const Rect& b) {
    float INTERSECTION = (a & b).area();
    float UNION = a.area() + b.area();
    return INTERSECTION / (UNION - INTERSECTION);
}

bool comp(const pair<int, float>& a, const pair<int, float>& b) {
    return a.second > b.second;
}

void nms(const vector<Rect>& boxes, const vector<float>& probabilities,
    float threshold, vector<unsigned>& indices) {

    vector <pair<int, float> > tmp;
    for (int i = 0; i < boxes.size(); i++) {
        tmp.push_back(make_pair(i, probabilities[i]));
    }
    for (int i = 0; i < boxes.size(); i++) {
        for (int j = i + 1; j < boxes.size(); j++) {
            if (iou(boxes[i], boxes[j]) > threshold) {
                if (tmp[i].second > tmp[j].second) {
                    tmp[j].first = -1;
                }
                else {
                    tmp[i].first = -1;
                    break;
                }
            }
        }
        sort(tmp.begin(), tmp.end(), comp);
    }
    for (int i = 0; i < boxes.size(); i++) {
        if (tmp[i].first != -1) {
            indices.push_back(tmp[i].first);
        }
    }
}