#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class PerlinNoise {
private:
    std::vector<int> p;

    float fade(float t) const;
    float lerp(float a, float b, float t) const;
    int fastFloor(float x) const;
    float grad(int hash, float x, float y, float z) const;

public:
    PerlinNoise(unsigned long long seed = 0);
    
    void reseed(unsigned long long newSeed);
    
    float noise(float x, float y) const;
    
    float octaveNoise(float x, float y, int octaves, float persistence) const;
    
    std::vector<std::vector<float>> generateNoiseMap(
        int width, int height, float scale,
        int octaves, float persistence, 
        float lacunarity, float offsetX, 
        float offsetY, bool seamless) const;
    
    std::vector<std::vector<float>> normalizeNoiseMap(
        const std::vector<std::vector<float>>& noiseMap, 
        float minValue, float maxValue);
    
    // Specialized function for cave generation that creates more connected tunnels
    float caveNoise(float x, float y, float z, float worminess = 0.5) {
        // Use multiple noise layers at different frequencies
        float n1 = noise(x, y * worminess);
        float n2 = noise(x * 2.0f, y * 2.0f * worminess) * 0.5f;
        float n3 = noise(x * 4.0f, y * 4.0f * worminess) * 0.25f;
        
        // Use z parameter for additional dimension if needed
        float zFactor = z * 0.1f;  // Reduce z influence
        float n4 = noise(x + zFactor, y + zFactor) * 0.125f;
        
        // Combine noise layers
        float combinedNoise = n1 + n2 + n3 + n4;
        
        // Apply a transformation to create more tunnel-like structures
        // This creates sharper transitions between cave and solid
        return (std::tanh(combinedNoise * 2.0f - 1.0f) + 1.0f) * 0.5f;
    }
}; 