#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// Structure to represent a pixel
struct pixel {
    int r, g, b;
};

// Structure to represent a centroid
struct centroid {
    double r, g, b;
};

// Function to calculate the squared distance between a pixel and a centroid
double pixelDistance(const pixel& p, const centroid& c) {
    double dr = static_cast<double>(p.r) - c.r;
    double dg = static_cast<double>(p.g) - c.g;
    double db = static_cast<double>(p.b) - c.b;
    return dr * dr + dg * dg + db * db;
}

// Function to initialize centroids randomly
vector<centroid> initializeCentroids(const vector<pixel>& pixels, int K) {
    vector<centroid> centroids;
    centroids.reserve(K);
    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < K; i++) {
        int idx = rand() % pixels.size();
        centroids.push_back(centroid{
            static_cast<double>(pixels[idx].r),
            static_cast<double>(pixels[idx].g),
            static_cast<double>(pixels[idx].b)
        });
    }
    return centroids;
}

// K-Means Clustering Function
vector<centroid> kMeans(const vector<pixel>& pixels, int K, int maxIterations = 100) {
    vector<centroid> centroids = initializeCentroids(pixels, K);
    vector<int> labels(pixels.size(), -1);

    for (int iter = 0; iter < maxIterations; iter++) {
        bool changed = false;

        // Assignment Step
        for (size_t i = 0; i < pixels.size(); i++) {
            double minDist = numeric_limits<double>::max();
            int bestCluster = -1;

            for (int j = 0; j < K; j++) {
                double dist = pixelDistance(pixels[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }

            if (labels[i] != bestCluster) {
                labels[i] = bestCluster;
                changed = true;
            }
        }

        // Update Step
        vector<centroid> newCentroids(K, {0.0, 0.0, 0.0});
        vector<int> counts(K, 0);

        for (size_t i = 0; i < pixels.size(); i++) {
            int cluster = labels[i];
            newCentroids[cluster].r += pixels[i].r;
            newCentroids[cluster].g += pixels[i].g;
            newCentroids[cluster].b += pixels[i].b;
            counts[cluster]++;
        }

        for (int j = 0; j < K; j++) {
            if (counts[j] > 0) {
                newCentroids[j].r /= counts[j];
                newCentroids[j].g /= counts[j];
                newCentroids[j].b /= counts[j];
            }
        }

        if (!changed) break;
        centroids = newCentroids;
    }

    return centroids;
}

// Function to print the terminal theme
void printTerminalTheme(const vector<centroid>& colors) {
    cout << "Generated Terminal Theme:\n\n";
    for (size_t i = 0; i < colors.size(); i++) {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        // Print color block
        cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"
             << "  Color " << i + 1 << "  "
             << "\033[0m RGB: (" << r << ", " << g << ", " << b << ")\n";
    }
}

int main() {
    int width, height, channels;
    vector<pixel> pixels;

    cout << "Enter the file path to the image: ";
    string filename;
    cin >> filename;

    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (img == NULL) {
        cout << "Couldn't load the image.\n";
        return 1;
    }

    cout << "Loaded " << filename << " with width " << width << " px and height " << height << " px.\n";

    pixels.resize(width * height);
    for (int i = 0; i < width * height; i++) {
        pixels[i].r = img[3 * i + 0];
        pixels[i].g = img[3 * i + 1];
        pixels[i].b = img[3 * i + 2];
    }
    stbi_image_free(img);

    int K;
    cout << "Enter the number of clusters to find: ";
    cin >> K;
    if (K <= 0 || K > width * height) {
        cout << "Invalid number of clusters.\n";
        return 1;
    }

    vector<centroid> dominantColors = kMeans(pixels, K);
    printTerminalTheme(dominantColors);

    return 0;
}
