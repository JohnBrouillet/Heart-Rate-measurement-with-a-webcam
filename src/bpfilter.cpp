#include "include/bpfilter.h"

std::vector<double> BPFilter::get_filtered_signal(std::vector<double>& signal)
{
    std::vector<double> filtered_signal;
    for(const double& value : signal)
    {
        SampleFilter_put(&filter, value);
        filtered_signal.push_back(SampleFilter_get(&filter));
    }

    return filtered_signal;
}


void SampleFilter_init(SampleFilter* f) {
  int i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, double input) {
  f->history[f->last_index++] = input;
  if(f->last_index == SAMPLEFILTER_TAP_NUM)
    f->last_index = 0;
}

double SampleFilter_get(SampleFilter* f) {
  double acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
    acc += f->history[index] * filter_taps[i];
    index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
  };
  return acc;
}
