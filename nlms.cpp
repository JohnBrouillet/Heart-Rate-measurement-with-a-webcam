#include "nlms.h"


std::vector<double> NLMS::compute_nlms(std::vector<double> d, std::vector<double> x, double alpha)
{
    std::vector<double> filtered_signal;
    for(int i = 0; i < d.size(); i++)
        filtered_signal.push_back(nlms_step(d[i], x[i], alpha));

    return filtered_signal;
}

double NLMS::nlms_step(double d, double x, double alpha)
{
    double e = d - h*x;
    h += alpha*x*e/(x*x+0.00001f);
    return e;
}
