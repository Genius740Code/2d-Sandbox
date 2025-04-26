#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "engine/PerlinNoise.h"

// Save noise map to a text file for visualization
void saveNoiseMapToFile(const std::vector<std::vector<float>>& noiseMap, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Get dimensions
    int width = noiseMap.size();
    int height = noiseMap[0].size();
    
    // Write noise values
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Map noise values to ASCII characters for simple visualization
            float value = noiseMap[x][y];
            char c;
            
            if (value < 0.2f) c = '~';      // Deep water
            else if (value < 0.4f) c = '=';  // Shallow water
            else if (value < 0.5f) c = '.';  // Sand
            else if (value < 0.7f) c = '"';  // Grass
            else if (value < 0.8f) c = '*';  // Forest
            else if (value < 0.9f) c = '^';  // Mountain
            else c = '#';                    // Snow
            
            outFile << c;
        }
        outFile << std::endl;
    }
    
    outFile.close();
    std::cout << "Noise map saved as ASCII art to " << filename << std::endl;
}

// Save as raw values for more detailed analysis
void saveRawNoiseMapToFile(const std::vector<std::vector<float>>& noiseMap, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Get dimensions
    int width = noiseMap.size();
    int height = noiseMap[0].size();
    
    // Write dimensions first
    outFile << width << " " << height << std::endl;
    
    // Write raw noise values
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            outFile << noiseMap[x][y] << " ";
        }
        outFile << std::endl;
    }
    
    outFile.close();
    std::cout << "Raw noise values saved to " << filename << std::endl;
}

int main() {
    // Seed with a value between 1 billion and 100 billion
    unsigned long long seed = 12345678901ULL;
    std::cout << "Testing Perlin noise generator with seed: " << seed << std::endl;
    
    PerlinNoise noiseGen(seed);
    
    // Test parameters
    const int mapWidth = 80;  // Width of the noise map (good for console display)
    const int mapHeight = 40; // Height of the noise map
    const float scale = 20.0f; // Lower value = more zoomed out terrain
    
    std::cout << "Generating standard noise map..." << std::endl;
    
    // Generate a standard noise map
    auto noiseMap = noiseGen.generateNoiseMap(
        mapWidth, mapHeight, scale, 4, 0.5f, 2.0f);
    
    // Normalize to 0.0-1.0 range
    noiseMap = PerlinNoise::normalizeNoiseMap(noiseMap);
    
    // Save the noise map
    saveNoiseMapToFile(noiseMap, "terrain_ascii.txt");
    saveRawNoiseMapToFile(noiseMap, "terrain_raw.txt");
    
    // Print a sample directly to console
    std::cout << "\nTerrain sample (20x10):" << std::endl;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {
            float value = noiseMap[x][y];
            char c;
            
            if (value < 0.2f) c = '~';      // Deep water
            else if (value < 0.4f) c = '=';  // Shallow water
            else if (value < 0.5f) c = '.';  // Sand
            else if (value < 0.7f) c = '"';  // Grass
            else if (value < 0.8f) c = '*';  // Forest
            else if (value < 0.9f) c = '^';  // Mountain
            else c = '#';                    // Snow
            
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    
    // Generate a second noise map with different parameters
    std::cout << "\nGenerating mountainous terrain..." << std::endl;
    
    // Use a different seed
    noiseGen.reseed(98765432101ULL);
    
    // More octaves and higher persistence for rougher terrain
    auto mountainNoiseMap = noiseGen.generateNoiseMap(
        mapWidth, mapHeight, scale * 0.5f, 6, 0.65f, 2.2f);
    mountainNoiseMap = PerlinNoise::normalizeNoiseMap(mountainNoiseMap);
    saveNoiseMapToFile(mountainNoiseMap, "mountains_ascii.txt");
    
    // Generate a seamless tileable noise map
    std::cout << "\nGenerating seamless tileable terrain..." << std::endl;
    auto seamlessNoiseMap = noiseGen.generateNoiseMap(
        mapWidth, mapHeight, scale, 4, 0.5f, 2.0f, 0.0f, 0.0f, true);
    seamlessNoiseMap = PerlinNoise::normalizeNoiseMap(seamlessNoiseMap);
    saveNoiseMapToFile(seamlessNoiseMap, "seamless_terrain_ascii.txt");
    
    std::cout << "\nTesting complete! Check the generated text files for results." << std::endl;
    std::cout << "Files created:" << std::endl;
    std::cout << "- terrain_ascii.txt (ASCII visualization)" << std::endl;
    std::cout << "- terrain_raw.txt (Raw noise values)" << std::endl;
    std::cout << "- mountains_ascii.txt (Mountainous terrain)" << std::endl;
    std::cout << "- seamless_terrain_ascii.txt (Tileable terrain)" << std::endl;
    
    return 0;
} 