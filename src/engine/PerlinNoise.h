#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class PerlinNoise {
private:
    std::vector<int> p;

    double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    PerlinNoise(int seed = 0) {
        p.resize(512);
        
        // Create permutation array
        std::vector<int> permutation(256);
        for(int i = 0; i < 256; i++)
            permutation[i] = i;
            
        // Use seed to shuffle the permutation array
        std::mt19937 engine(seed);
        std::shuffle(permutation.begin(), permutation.end(), engine);
        
        // Duplicate the permutation array
        for(int i = 0; i < 256; i++) {
            p[i] = permutation[i];
            p[256+i] = permutation[i];
        }
    }

    double noise(double x, double y, double z = 0.0) {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        int A = p[X] + Y;
        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int B = p[X + 1] + Y;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                     grad(p[BA], x-1, y, z)),
                             lerp(u, grad(p[AB], x, y-1, z),
                                  grad(p[BB], x-1, y-1, z))),
                     lerp(v, lerp(u, grad(p[AA+1], x, y, z-1),
                                  grad(p[BA+1], x-1, y, z-1)),
                          lerp(u, grad(p[AB+1], x, y-1, z-1),
                               grad(p[BB+1], x-1, y-1, z-1))));
    }
    
    // Function to generate octaved noise (multiple frequencies combined)
    double octaveNoise(double x, double y, int octaves, double persistence) {
        double total = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double maxValue = 0.0;
        
        for(int i = 0; i < octaves; i++) {
            total += noise(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2.0;
        }
        
        // Normalize to 0-1 range
        return total / maxValue;
    }
    
    // Specialized function for cave generation that creates more connected tunnels
    double caveNoise(double x, double y, double z, double worminess = 0.5) {
        // Use multiple noise layers at different frequencies
        double n1 = noise(x, y * worminess, z);
        double n2 = noise(x * 2.0, y * 2.0 * worminess, z * 2.0) * 0.5;
        double n3 = noise(x * 4.0, y * 4.0 * worminess, z * 4.0) * 0.25;
        
        // Combine noise layers
        double combinedNoise = n1 + n2 + n3;
        
        // Apply a transformation to create more tunnel-like structures
        // This creates sharper transitions between cave and solid
        return (std::tanh(combinedNoise * 2.0 - 1.0) + 1.0) * 0.5;
    }
}; 