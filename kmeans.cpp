#include "kmeans.h"
#include <limits>
#include <cstdlib>
#include <ctime>
#include <cmath>

static double pixelDistance(const pixel &p, const centroid &c) {
    double dr = p.r - c.r;
    double dg = p.g - c.g;
    double db = p.b - c.b;
    return dr*dr + dg*dg + db*db;
}

// Randomly pick initial centroids from the pixels
static std::vector<centroid> initializeCentroids(const std::vector<pixel> &pixels, int K) {
    std::vector<centroid> centroids;
    centroids.reserve(K);
    std::srand((unsigned)std::time(nullptr));
    for (int i = 0; i < K; i++) {
        int idx = std::rand() % pixels.size();
        centroids.push_back({
            static_cast<double>(pixels[idx].r),
            static_cast<double>(pixels[idx].g),
            static_cast<double>(pixels[idx].b)
        });
    }
    return centroids;
}

std::vector<centroid> kMeans(const std::vector<pixel> &pixels, int K, int maxIterations) {
    auto centroids = initializeCentroids(pixels, K);
    std::vector<int> labels(pixels.size(), -1);

    for (int iter = 0; iter < maxIterations; iter++) {
        bool changed = false;
        // Assignment step
        for (size_t i = 0; i < pixels.size(); i++) {
            double minDist = std::numeric_limits<double>::max();
            int best = -1;
            for (int j = 0; j < K; j++) {
                double dist = pixelDistance(pixels[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    best = j;
                }
            }
            if (labels[i] != best) {
                labels[i] = best;
                changed = true;
            }
        }
        // Early stop if no changes
        if (!changed) break;

        // Update step
        std::vector<centroid> newC(K, {0, 0, 0});
        std::vector<int> counts(K, 0);

        for (size_t i = 0; i < pixels.size(); i++) {
            newC[labels[i]].r += pixels[i].r;
            newC[labels[i]].g += pixels[i].g;
            newC[labels[i]].b += pixels[i].b;
            counts[labels[i]]++;
        }
        for (int j = 0; j < K; j++) {
            if (counts[j] > 0) {
                newC[j].r /= counts[j];
                newC[j].g /= counts[j];
                newC[j].b /= counts[j];
            }
        }
        centroids = newC;
    }
    return centroids;
}
