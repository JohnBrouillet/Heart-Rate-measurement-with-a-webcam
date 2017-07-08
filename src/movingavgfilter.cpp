#include "include/movingavgfilter.h"

movingAvgFilter::movingAvgFilter(int _M)
{
    M = (_M % 2) ? _M : _M+1;
}

std::vector<double> movingAvgFilter::filter(std::vector<double> signal)
{
    std::vector<double> y;
    double acc = std::accumulate(signal.begin(), signal.begin()+M, 0.0);
    y.push_back(acc/M);

    int m = std::ceil(M/2);
    for(int i = m; i < signal.size()-m ; i++)
    {
        acc += signal[i+m-1] - signal[i-m];
        y.push_back(acc/M);
    }

    return y;
}
