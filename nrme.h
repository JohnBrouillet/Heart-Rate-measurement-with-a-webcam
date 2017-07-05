#ifndef NRME_H
#define NRME_H

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>

class NRME
{
public:
    NRME(int _nb_segment);
    std::vector<double> nrme(std::vector<double> signal);

private:
    int nb_segment;
    int five_percent;

    std::vector<std::vector<double> > vector_segmentation(const std::vector<double>& v, const int& segment_size);
    std::vector<std::pair<int, double>> stddev_sort(const  std::vector<std::vector<double> >& segmented_signal);
    std::vector<int> get_sorted_index(const std::vector<std::pair<int, double>>& pair);
    void segment_elimination(std::vector<double>& signal, const std::vector<int> id, const int& segment_size);
    double stddev(const std::vector<double>& v);

};

#endif // NRME_H
