#include <iostream>
#include <vector>
#include <string>
#include "pixel.h"
#include "kmeans.h"
#include "median_cut.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Load an image from file and convert it to a vector of pixels
std::vector<pixel> loadImage(const std::string& filename, int& width, int& height) {
    int channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (!img) {
        std::cerr << "Could not load image.\n";
        exit(1);
    }

    std::vector<pixel> pixels(width * height);
    for (int i = 0; i < width * height; i++) {
        pixels[i].r = img[3 * i + 0];
        pixels[i].g = img[3 * i + 1];
        pixels[i].b = img[3 * i + 2];
    }
    stbi_image_free(img);

    std::cout << "Loaded " << filename << " (" << width << "x" << height << ")\n";
    return pixels;
}


int getNumberOfColors() {
    int K;
    std::cout << "How many clusters/colors do you want? ";
    std::cin >> K;
    if (K <= 0) {
        std::cout << "Invalid number of colors.\n";
        exit(1);
    }
    return K;
}

int getAlgorithmChoice() {
    std::cout << "Choose algorithm:\n"
              << "  1) K-Means\n"
              << "  2) Median Cut\n";
    int choice;
    std::cin >> choice;
    if (choice != 1 && choice != 2) {
        std::cout << "Invalid choice.\n";
        exit(1);
    }
    return choice;
}

// Generate a color palette using the chosen algorithm
std::vector<centroid> generateColorPalette(const std::vector<pixel>& pixels, int K, int choice) {
    if (choice == 1) {
        std::cout << "How many iterations? \n";
        int maxIterations;
        std::cin >> maxIterations;
        return kMeans(pixels, K, maxIterations);
    } else {
        return medianCutQuantize(pixels, K);
    }
}

// Print the generated color palette in ANSI escape codes
void printColorPalette(const std::vector<centroid>& colorPalette) {
    const std::string reset = "\033[0m";
    for (size_t i = 0; i < colorPalette.size(); i++) {
        int r = (int)colorPalette[i].r;
        int g = (int)colorPalette[i].g;
        int b = (int)colorPalette[i].b;
        std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"
                  << "  Color " << i + 1 << "  "
                  << "\033[0m RGB: (" << r << ", " << g << ", " << b << ")\n";
    }
}

int main() {
    std::cout << "Enter image path: ";
    std::string filename;
    std::cin >> filename;

    int width, height;
    std::vector<pixel> pixels = loadImage(filename, width, height);

    int K = getNumberOfColors();
    int choice = getAlgorithmChoice();

    std::vector<centroid> colorPalette = generateColorPalette(pixels, K, choice);

    printColorPalette(colorPalette);

    return 0;
}
