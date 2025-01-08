#include <iostream>
#include <vector>
#include <string>
#include "pixel.h"
#include "kmeans.h"
#include "median_cut.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    // Load image
    std::cout << "Enter image path: ";
    std::string filename;
    std::cin >> filename;

    int width, height, channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (!img) {
        std::cerr << "Could not load image.\n";
        return 1;
    }

    // Convert to pixel struct
    std::vector<pixel> pixels(width * height);
    for (int i = 0; i < width * height; i++) {
        pixels[i].r = img[3*i + 0];
        pixels[i].g = img[3*i + 1];
        pixels[i].b = img[3*i + 2];
    }
    stbi_image_free(img);

    std::cout << "Loaded " << filename
              << " (" << width << "x" << height << ")\n";

    // Prompt for number of colors
    int K;
    std::cout << "How many clusters/colors do you want? ";
    std::cin >> K;
    if (K <= 0 || K > (width * height)) {
        std::cerr << "Invalid K.\n";
        return 1;
    }

    // Prompt which algorithm to use
    std::cout << "Choose algorithm:\n"
              << "  1) K-Means\n"
              << "  2) Median Cut\n";
    int choice;
    std::cin >> choice;

    std::vector<centroid> colorPalette;
    if (choice == 1) {
        colorPalette = kMeans(pixels, K);  // uses default maxIterations=100
    } else if (choice == 2) {
        colorPalette = medianCutQuantize(pixels, K);
    } else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    // Print out the resulting colors
    for (size_t i = 0; i < colorPalette.size(); i++) {
        int r = (int)colorPalette[i].r;
        int g = (int)colorPalette[i].g;
        int b = (int)colorPalette[i].b;
        // Print color block
        cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"
             << "  Color " << i + 1 << "  "
             << "\033[0m RGB: (" << r << ", " << g << ", " << b << ")\n";
    }


 


    return 0;
}
