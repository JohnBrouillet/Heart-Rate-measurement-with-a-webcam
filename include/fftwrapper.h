#ifndef FFTWRAPPER_H
#define FFTWRAPPER_H


#include <vector>
#include <armadillo>

class FFTWrapper
{
public:
    static std::vector<double> compute_rfft(std::vector<double> data, int NFFT);
    static double get_freq_highest_peak(std::vector<double>& fft, int NFFT, float fech);

private:
    static std::vector<double> get_FFT_norm(arma::Col<std::complex<double> >& cx_fft);
};

#endif // FFTWRAPPER_H
