#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "pixel.h"

std::vector<centroid> kMeans(const std::vector<pixel> &pixels, int K, int maxIterations = 100);

#endif
