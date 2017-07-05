#ifndef MOVINGAVGFILTER_H
#define MOVINGAVGFILTER_H

#include <vector>
#include <numeric>
#include <cmath>

class movingAvgFilter
{
public:
    movingAvgFilter(int _M);
    std::vector<double> filter(std::vector<double> signal);

private:
    int M;
};

#endif // MOVINGAVGFILTER_H
