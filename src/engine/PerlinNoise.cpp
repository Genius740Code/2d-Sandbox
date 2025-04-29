#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(unsigned long long seed) {
    reseed(seed);
}

void PerlinNoise::reseed(unsigned long long newSeed) {
    newSeed = std::max(1000000000ULL, std::min(newSeed, 100000000000ULL));
    
    p.resize(512);
    std::vector<int> source(256);
    for (int i = 0; i < 256; ++i) {
        source[i] = i;
    }
    
    // Seed the random number generator
    std::mt19937_64 engine(newSeed);
    
    std::shuffle(source.begin(), source.end(), engine);
    
    for (int i = 0; i < 256; ++i) {
        p[i] = p[i + 256] = source[i];
    }
}

float PerlinNoise::fade(float t) const {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float a, float b, float t) const {
    return a + t * (b - a);
}

int PerlinNoise::fastFloor(float x) const {
    return x > 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}

float PerlinNoise::grad(int hash, float x, float y, float z) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14) ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::noise(float x, float y) const {
    const float z = 0.0f;
    
    // Find unit cube that contains the point
    int X = fastFloor(x) & 255;
    int Y = fastFloor(y) & 255;
    int Z = fastFloor(z) & 255;
    
    // Create mutable copies for the coordinates
    float xm = x - fastFloor(x);
    float ym = y - fastFloor(y);
    float zm = z - fastFloor(z);
    
    // Compute fade curves for each of x, y, z
    float u = fade(xm);
    float v = fade(ym);
    float w = fade(zm);
    
    // Hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;
    
    // Add blended results from 8 corners of cube
    float res = lerp(lerp(lerp(grad(p[AA], xm, ym, zm),
                              grad(p[BA], xm - 1, ym, zm), u),
                         lerp(grad(p[AB], xm, ym - 1, zm),
                              grad(p[BB], xm - 1, ym - 1, zm), u), v),
                    lerp(lerp(grad(p[AA + 1], xm, ym, zm - 1),
                              grad(p[BA + 1], xm - 1, ym, zm - 1), u),
                         lerp(grad(p[AB + 1], xm, ym - 1, zm - 1),
                              grad(p[BB + 1], xm - 1, ym - 1, zm - 1), u), v), w);
    
    return res;
}

float PerlinNoise::octaveNoise(float x, float y, int octaves, float persistence) const {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;  
    
    for (int i = 0; i < octaves; ++i) {
        total += noise(x * frequency, y * frequency) * amplitude;
        
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    
    return total / maxValue;
}

std::vector<std::vector<float>> PerlinNoise::generateNoiseMap(int width, int height, float scale,
                                                             int octaves, float persistence, 
                                                             float lacunarity, float offsetX, 
                                                             float offsetY, bool seamless) const {
    std::vector<std::vector<float>> noiseMap(width, std::vector<float>(height, 0.0f));
    
    if (scale <= 0) {
        scale = 0.0001f;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> offsetDist(-10000.0f, 10000.0f);
    
    std::vector<float> octaveOffsets(octaves * 2);
    for (int i = 0; i < octaves * 2; i += 2) {
        octaveOffsets[i] = offsetDist(gen) + offsetX;
        octaveOffsets[i + 1] = offsetDist(gen) + offsetY;
    }
    
    auto sampleNoise = [&](float x, float y) -> float {
        float noise = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float maxValue = 0.0f;
        
        for (int i = 0; i < octaves; ++i) {
            float sampleX = (x / scale) * frequency + octaveOffsets[i * 2];
            float sampleY = (y / scale) * frequency + octaveOffsets[i * 2 + 1];
            
            float perlinValue;
            if (seamless) {
                float nx = sampleX / width;
                float ny = sampleY / height;
                float dx = 1.0f - nx;
                float dy = 1.0f - ny;
                
                perlinValue = 
                    this->noise(sampleX, sampleY) * (nx * ny) + 
                    this->noise(sampleX - width, sampleY) * (dx * ny) + 
                    this->noise(sampleX, sampleY - height) * (nx * dy) + 
                    this->noise(sampleX - width, sampleY - height) * (dx * dy);
            } else {
                perlinValue = this->noise(sampleX, sampleY);
            }
            
            noise += perlinValue * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }
        
        return noise / maxValue;
    };
    
    // Multi-threaded version - disabled to avoid compiler warnings
    // #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            noiseMap[x][y] = sampleNoise(static_cast<float>(x), static_cast<float>(y));
        }
    }
    
    return noiseMap;
}

std::vector<std::vector<float>> PerlinNoise::normalizeNoiseMap(
    const std::vector<std::vector<float>>& noiseMap, 
    float minValue, float maxValue) {
    
    if (noiseMap.empty() || noiseMap[0].empty()) {
        return noiseMap;
    }
    
    // Find min and max noise values
    float min = noiseMap[0][0];
    float max = noiseMap[0][0];
    
    for (const auto& row : noiseMap) {
        for (float value : row) {
            min = std::min(min, value);
            max = std::max(max, value);
        }
    }
    
    if (max == min) {
        return noiseMap;
    }
    
    // Create normalized copy
    std::vector<std::vector<float>> normalizedMap = noiseMap;
    float range = maxValue - minValue;
    
    for (size_t i = 0; i < normalizedMap.size(); ++i) {
        for (size_t j = 0; j < normalizedMap[i].size(); ++j) {
            // Normalize to [0,1]
            normalizedMap[i][j] = (normalizedMap[i][j] - min) / (max - min);
            // Scale to desired range
            normalizedMap[i][j] = minValue + normalizedMap[i][j] * range;
        }
    }
    
    return normalizedMap;
} 