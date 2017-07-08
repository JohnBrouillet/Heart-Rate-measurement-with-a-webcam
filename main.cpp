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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>
#include <pybind11/embed.h>

#include <QDir>
namespace py = pybind11;

const int nb_samples = 600; // 20 seconds
const int nb_segment = 10;
const int movingAvgSize = 11;

int main()
{
    Py_Initialize();

    QDir sourcedir("/home/neo/Documents/cpp/detection-cardiaque/HRLib/HRLib/Heart-rate-measurement-with-a-webcam/python/plot/");
    py::module sysmod = py::module::import("sys");
    py::function sysappend = sysmod.attr("path").attr("append").cast<py::function>();
    sysappend(sourcedir.absolutePath().toStdString());

    auto mymod = py::module::import("plot");
    auto plot_all = mymod.attr("plot_all");

    cv::Mat img;
    std::vector<cv::Mat> data;
    cv::VideoCapture cap(0);
    cap >> img;

    std::cout << "We wait 3 seconds for the camera to stabilize" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Let's go! Don't move!" << std::endl;

    for(int i = 0; i < nb_samples; ++i)
    {      
        cv::Mat img;
        cap >> img;
        data.push_back(img);
    }
    cap.release();
    std::cout << "Acquisition done! "<< std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<double> signal_roi, signal_background;
    SignalExtractor extractor;
    for(int i = 0; i < data.size(); i++)
    {
        std::pair<double, double> res = extractor.compute_frame(data[i]);
        signal_roi.push_back(res.first);
        signal_background.push_back(res.second);
    }

    std::cout << "Computation" << std::endl;

    std::vector<double> desired = signal_roi;
    std::vector<double> noise = signal_background;

    NLMS nlms;
    std::vector<double> post_nlms;
    post_nlms = nlms.compute_nlms(desired, noise, 0.1f);

    NRME nrme(nb_segment);
    std::vector<double> post_nrme;
    post_nrme = nrme.nrme(post_nlms);

    auto detrending = [](std::vector<double>& vec){
      std::vector<double> detrend;
      detrend.push_back(0);
      for(int i = 1; i < vec.size(); i++)
          detrend.push_back(vec[i] - vec[i-1] + 0.9*detrend[i-1]);

      return detrend;
    };

    std::vector<double> detrend;
    detrend = detrending(post_nrme);

    movingAvgFilter avg(movingAvgSize);
    std::vector<double> mvAvg;
    mvAvg = avg.filter(detrend);

    BPFilter filtre;
    std::vector<double> filtered;
    filtered = filtre.get_filtered_signal(mvAvg);

    int NFFT = 1024;
    std::vector<double> post_fft = FFTWrapper::compute_rfft(filtered, NFFT);

    double HR = FFTWrapper::get_freq_highest_peak(post_fft, NFFT, 30);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

    std::cout << "Finished !" << std::endl;
    std::cout << "HR : " << HR << "Hz = " << HR*60 << "bpm" << std::endl;
    std::cout << "Processing time: " << fp_ms.count() << "ms" << std::endl;

    plot_all(desired, post_nlms, post_nrme, detrend, mvAvg, filtered, post_fft, NFFT);

    return 0;
}
