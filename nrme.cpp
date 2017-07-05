#include "nrme.h"

NRME::NRME(int _nb_segment){
    nb_segment = _nb_segment;
    five_percent = std::ceil(nb_segment*5.0f/100.0f);
}

std::vector<double> NRME::nrme(std::vector<double> signal)
{
    int segment_size = int(signal.size() / nb_segment);

    std::vector<std::vector<double> > segmented_signal = vector_segmentation(signal, segment_size);
    std::vector<std::pair<int, double>> pair_index_stddev = stddev_sort(segmented_signal);
    std::vector<int> id = get_sorted_index(pair_index_stddev);
    segment_elimination(signal, id, segment_size);

    return signal;
}

std::vector<std::vector<double> > NRME::vector_segmentation(const std::vector<double>& v, const int& segment_size)
{
    std::vector<std::vector<double> > segmented_signal;
    for(int i = 0; i < nb_segment; i++)
        segmented_signal.push_back(std::vector<double>(&v[i*segment_size], &v[(i+1)*segment_size - 1]));

    return segmented_signal;
}

std::vector<std::pair<int, double>> NRME::stddev_sort(const  std::vector<std::vector<double> >& segmented_signal)
{
    std::vector<std::pair<int, double>> pair_index_stddev;
    for(int i = 0; i < segmented_signal.size(); i++)
        pair_index_stddev.push_back(std::make_pair(i, stddev(segmented_signal[i])));

    std::sort(pair_index_stddev.begin(), pair_index_stddev.end(), [](const std::pair<int,double>& i, const std::pair<int,double>& j) { return (i.second>j.second); });

    return pair_index_stddev;
}

std::vector<int> NRME::get_sorted_index(const std::vector<std::pair<int, double>>& pair)
{
    std::vector<int> id;
    for(int i = 0; i < five_percent; i++)
        id.push_back(pair[i].first);

    std::sort(id.begin(), id.end(), [](const int& i, const int&j){ return i > j;});

    return id;
}

void NRME::segment_elimination(std::vector<double>& signal, const std::vector<int> id, const int& segment_size)
{
    for(int i = 0; i < five_percent; i++)
        signal.erase(signal.begin()+id[i]*segment_size, signal.begin() + (id[i]+1)*segment_size);
}

double NRME::stddev(const std::vector<double>& v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0f);
    double mean = sum / v.size();

    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0f);
    double stdev = std::sqrt(sq_sum / v.size());

    return stdev;
}
