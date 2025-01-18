#ifndef MEDIAN_CUT_H
#define MEDIAN_CUT_H

#include <vector>
#include "pixel.h"

std::vector<centroid> medianCutQuantize(const std::vector<pixel> &pixels, int K);

#endif
