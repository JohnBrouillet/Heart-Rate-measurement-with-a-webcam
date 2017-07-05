#include "signalextractor.h"

SignalExtractor::SignalExtractor() : id{1, 3, 6, 8, 10, 13, 15, 41, 47},
    offset_x{10, 7, 0, 0, 0, -7, -10, 0, 0},  offset_y{0, 0, -10, -10, -10, 0, 0, 10, 10}
{
    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
    count = 0;
    start = std::chrono::steady_clock::now();
}

std::pair<double,double> SignalExtractor::compute_frame(cv::Mat img)
{
    cv::resize(img, img, cv::Size(), 1.0/FACE_DOWNSAMPLE_RATIO, 1.0/FACE_DOWNSAMPLE_RATIO);
    mask = cv::Mat::zeros(img.rows,img.cols,CV_8UC1);

    dlib::cv_image<dlib::bgr_pixel> cimg(img);
    std::vector<dlib::rectangle> faces = detector(cimg);
    dlib::full_object_detection shapes;

    if(!(faces.empty()))
    {
        shapes = pose_model(cimg, faces[0]);

        std::vector<int> roi, background;
        getGreenVectors(img, shapes, roi, background);
        std::pair<double,double> result = computeMean(roi, background);

        return result;
    }
    else
        return std::make_pair<double, double>(0.0, 0.0);
}

std::pair<std::vector<double>, std::vector<double> > SignalExtractor::getVectors()
{
    std::pair<std::vector<double>, std::vector<double> > roi_background_pair = std::make_pair(signal_face, signal_background);
    signal_face.clear();
    signal_background.clear();

    return roi_background_pair;
}

void SignalExtractor::getGreenVectors(const cv::Mat& img, dlib::full_object_detection &shapes, std::vector<int>& roi, std::vector<int>& background)
{
    std::vector<cv::Point> rook_points;
    for(int i = 0; i < NB_POINTS; ++i)
    {
        auto s = shapes.part(id[i]);
        rook_points.push_back(cv::Point(s.x() + offset_x[i], s.y() + offset_y[i]));
    }
    cv::circle(img,rook_points[0],2,CV_RGB(255,0,0));


    for(int i = 0; i < NB_POINTS; ++i)
        cv::line(mask,rook_points[i],rook_points[(i+1)%NB_POINTS],cv::Scalar(255),1);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::drawContours(mask,contours,0,cv::Scalar(255),CV_FILLED,1);

    for(int i = 1; i < img.rows*img.cols; i+=3)
        mask.data[i] == 255 ? roi.push_back(img.data[i]) : background.push_back(img.data[i]);
}

std::pair<double,double> SignalExtractor::computeMean(std::vector<int>& roi, std::vector<int>& background)
{
    auto mean = [](std::vector<int>& vec){
        double sum = std::accumulate(vec.begin(),vec.end(),0.0);
        return double(sum / vec.size());
    };

    return std::make_pair<double,double>(mean(roi), mean(background));
}
