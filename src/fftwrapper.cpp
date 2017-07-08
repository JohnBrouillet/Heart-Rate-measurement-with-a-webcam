#include "include/fftwrapper.h"

std::vector<double> FFTWrapper::compute_rfft(std::vector<double> data, int NFFT)
{
    arma::Col<std::complex<double> > dstArma = arma::fft(arma::Col<double>(data), NFFT);
    return get_FFT_norm(dstArma);
}

double FFTWrapper::get_freq_highest_peak(std::vector<double>& fft, int NFFT, float fech)
{
    int index = std::distance(fft.begin(), max_element(fft.begin()+1+0.7*NFFT/fech, fft.end()));
    return double(index) * fech / (double)NFFT;
}

std::vector<double> FFTWrapper::get_FFT_norm(arma::Col<std::complex<double> >& cx_fft)
{
    arma::Col<double> norm_fft = arma::abs(cx_fft);
    return std::vector<double>(norm_fft.begin(), norm_fft.begin()+norm_fft.size()/2);
}
