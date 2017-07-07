#include <iostream>
#include <chrono>
#include <thread>

#include "include/nrme.h"
#include "include/signalextractor.h"
#include "include/fftwrapper.h"
#include "include/bpfilter.h"
#include "include/nlms.h"
#include "include/movingavgfilter.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>
#include <pybind11/embed.h>

#include <QDir>
namespace py = pybind11;

int main()
{
    Py_Initialize();

    QDir sourcedir("/home/neo/Documents/cpp/detection-cardiaque/HRLib/HRLib/Heart-rate-measurement-with-a-webcam/python/plot/");
    py::module sysmod = py::module::import("sys");
    py::function sysappend = sysmod.attr("path").attr("append").cast<py::function>();
    sysappend(sourcedir.absolutePath().toStdString());

    auto mymod = py::module::import("plot");
    auto plot_all = mymod.attr("plot_all");

    SignalExtractor extractor;
    std::vector<cv::Mat> data;
    cv::VideoCapture cap("face.mp4");
   /* for(int i = 0; i < 500; i++)
    {
        cv::Mat img;
        cap >> img;
    }*/
    std::cout << "wait " << std::endl;
cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE);
   // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::cout << "get" << std::endl;
  /*  for(int i = 0; i < 600; i++)
    {
        cv::Mat img;
        cap >> img;
        data.push_back(img);
    }*/


    while(1)
    {
        cv::Mat img;
        cap >> img;

        if(img.empty())
            break;
        data.push_back(img);
        cv::imshow("MyVideo", img);
        if(cv::waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
          {

                   break;
          }
    }

    std::cout << "ok !" << std::endl;

    std::vector<double> signal_roi, signal_background;
    for(int i = 0; i < data.size(); i++)
    {
        std::pair<double, double> res = extractor.compute_frame(data[i]);
        signal_roi.push_back(res.first);
        signal_background.push_back(res.second);
    }

    std::cout << "I got the signal ! " << signal_roi.size() << std::endl;
    std::cout << "COMPUTING TO THE DEATH !" << std::endl;

    auto moyennage = [](std::vector<double> vec){

        auto mean = [](std::vector<double>& vec){
            double sum = std::accumulate(vec.begin(),vec.end(),0.0);
            return double(sum / vec.size());
        };

        double moy = mean(vec);
        std::for_each(vec.begin(), vec.end(), [&](double& d) { d-=moy;});
        return vec;
    };

    std::vector<double> d = (signal_roi);
    std::vector<double> x = (signal_background);

    NLMS nlms;
    std::vector<double> signal;
    signal = nlms.compute_nlms(d, x, 0.1f);

    NRME nrme(30);
    std::vector<double> post_nrme;
    post_nrme = nrme.nrme(signal);

    movingAvgFilter avg(11);
    std::vector<double> mvAvg;
    mvAvg = avg.filter(post_nrme);

    BPFilter filtre;
    std::vector<double> filtered;
    filtered = filtre.get_filtered_signal(mvAvg);


    int NFFT = 1024;
    std::vector<double> post_fft = FFTWrapper::compute_rfft(filtered, NFFT);

    double HR = FFTWrapper::get_freq_highest_peak(post_fft, NFFT, 30);

    std::cout << "Finished !" << std::endl;
    std::cout << "HR : " << HR << "Hz = " << HR*60 << "bpm" << std::endl;

    plot_all(x, signal, filtered, post_nrme, mvAvg, post_fft);

    return 0;
}
