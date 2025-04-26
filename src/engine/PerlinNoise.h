#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class PerlinNoise {
public:
    // Constructor with seed (1 to 100 billion as per requirements)
    explicit PerlinNoise(unsigned long long seed = 1);
    
    // Generate noise at coordinates (x,y) with customizable parameters
    float noise(float x, float y) const;
    
    // Generate noise with multiple octaves (fractal Brownian motion)
    float octaveNoise(float x, float y, int octaves, float persistence = 0.5f) const;
    
    // Generate seamless noise map for tiling
    std::vector<std::vector<float>> generateNoiseMap(int width, int height, float scale,
                                                    int octaves = 4, float persistence = 0.5f, 
                                                    float lacunarity = 2.0f, float offsetX = 0.0f, 
                                                    float offsetY = 0.0f, bool seamless = false) const;
    
    // Change the seed
    void reseed(unsigned long long newSeed);
    
    // Normalize the noise values to a specific range
    static std::vector<std::vector<float>> normalizeNoiseMap(const std::vector<std::vector<float>>& noiseMap, 
                                                           float minValue = 0.0f, float maxValue = 1.0f);

private:
    std::vector<int> p; // Permutation table
    
    // Helper functions
    float fade(float t) const;
    float lerp(float a, float b, float t) const;
    float grad(int hash, float x, float y, float z) const;
    int fastFloor(float x) const;
}; 