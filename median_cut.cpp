#include "median_cut.h"
#include <algorithm>
#include <cmath>

// Internal structure to hold a subset of pixels
struct ColorBox {
    std::vector<pixel> pixels;
    int minR, maxR, minG, maxG, minB, maxB;
};

// Recalc bounding box
static void recalcBounds(ColorBox &box) {
    box.minR = box.minG = box.minB = 255;
    box.maxR = box.maxG = box.maxB = 0;
    for (auto &p : box.pixels) {
        if (p.r < box.minR) box.minR = p.r;
        if (p.g < box.minG) box.minG = p.g;
        if (p.b < box.minB) box.minB = p.b;
        if (p.r > box.maxR) box.maxR = p.r;
        if (p.g > box.maxG) box.maxG = p.g;
        if (p.b > box.maxB) box.maxB = p.b;
    }
}

// Split a box into two along the largest dimension
static std::pair<ColorBox, ColorBox> splitBox(ColorBox &box) {
    recalcBounds(box);

    int rangeR = box.maxR - box.minR;
    int rangeG = box.maxG - box.minG;
    int rangeB = box.maxB - box.minB;

    // Pick channel with largest range
    char channel = 'r';
    if (rangeG > rangeR && rangeG > rangeB) channel = 'g';
    else if (rangeB > rangeR && rangeB > rangeG) channel = 'b';

    if (channel == 'r') {
        std::sort(box.pixels.begin(), box.pixels.end(),
                  [](auto &a, auto &b){ return a.r < b.r; });
    } else if (channel == 'g') {
        std::sort(box.pixels.begin(), box.pixels.end(),
                  [](auto &a, auto &b){ return a.g < b.g; });
    } else {
        std::sort(box.pixels.begin(), box.pixels.end(),
                  [](auto &a, auto &b){ return a.b < b.b; });
    }

    int medianIdx = (int)box.pixels.size() / 2;

    ColorBox box1, box2;
    box1.pixels.insert(box1.pixels.end(), box.pixels.begin(), box.pixels.begin() + medianIdx);
    box2.pixels.insert(box2.pixels.end(), box.pixels.begin() + medianIdx, box.pixels.end());

    recalcBounds(box1);
    recalcBounds(box2);
    return std::make_pair(box1, box2);
}

// Keep splitting boxes until we have K
static void medianCut(std::vector<ColorBox> &boxes, int K) {
    while ((int)boxes.size() < K) {
        // Find the box with the largest range sum
        int maxRangeIdx = 0;
        int maxVal = -1;
        for (int i = 0; i < (int)boxes.size(); i++) {
            int r = boxes[i].maxR - boxes[i].minR;
            int g = boxes[i].maxG - boxes[i].minG;
            int b = boxes[i].maxB - boxes[i].minB;
            int sum = r + g + b;
            if (sum > maxVal) {
                maxVal = sum;
                maxRangeIdx = i;
            }
        }

        auto bigBox = boxes[maxRangeIdx];
        boxes.erase(boxes.begin() + maxRangeIdx);

        auto newBoxes = splitBox(bigBox);
        if (!newBoxes.first.pixels.empty()) {
            boxes.push_back(newBoxes.first);
        }
        if (!newBoxes.second.pixels.empty()) {
            boxes.push_back(newBoxes.second);
        }
    }
}

std::vector<centroid> medianCutQuantize(const std::vector<pixel> &pixels, int K) {
    // Put all pixels in initial box
    ColorBox initial;
    initial.pixels = pixels;
    recalcBounds(initial);

    std::vector<ColorBox> boxes;
    boxes.push_back(initial);

    medianCut(boxes, K);

    // Compute average of each final box
    std::vector<centroid> result;
    result.reserve(boxes.size());
    for (auto &b : boxes) {
        if (!b.pixels.empty()) {
            long long sumR = 0, sumG = 0, sumB = 0;
            for (auto &p : b.pixels) {
                sumR += p.r;
                sumG += p.g;
                sumB += p.b;
            }
            double n = (double)b.pixels.size();
            centroid c;
            c.r = sumR / n;
            c.g = sumG / n;
            c.b = sumB / n;
            result.push_back(c);
        }
    }

    return result;
}
