#ifndef NLMS_H
#define NLMS_H

#include <vector>

class NLMS
{
public:
    NLMS(){h = 0.0;}
    std::vector<double> compute_nlms(std::vector<double> d, std::vector<double> x, double alpha);
    double nlms_step(double d, double x, double alpha);

private:
    double h;
};
#endif // NLMS_H
