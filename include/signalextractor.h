#ifndef SIGNALEXTRACTOR_H
#define SIGNALEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <iostream>
#include <algorithm>
#include <numeric>


const int NB_POINTS = 9;
const int COUNT_LIMIT = 30;

class SignalExtractor
{
public:
    SignalExtractor();
    std::pair<double,double> compute_frame(cv::Mat img);
    std::pair<std::vector<double>, std::vector<double> > getVectors();

private:
    void getGreenVectors(const cv::Mat& img, dlib::full_object_detection &shapes, std::vector<int>& roi, std::vector<int>& background);
    std::pair<double,double> computeMean(std::vector<int>& roi, std::vector<int>& background);

    int count;
    int id[NB_POINTS], offset_x[NB_POINTS], offset_y[NB_POINTS];

    std::chrono::time_point<std::chrono::steady_clock> start;

    std::vector<double> signal_face;
    std::vector<double> signal_background;

    dlib::shape_predictor pose_model;
    dlib::frontal_face_detector detector;
    std::vector<dlib::image_window::overlay_line> draw;

    cv::Mat mask;
};


#endif // SIGNALEXTRACTOR_H
