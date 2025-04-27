#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <cstdint>
#include "PerlinNoise.h"
#include "Chunk.h"
#include "TileManager.h"

class World {
private:
    static const int MAX_CHUNKS = 7;         // Maximum number of active chunks
    static const int CHUNK_WIDTH = 16;       // Width of a chunk in blocks
    static const int TOTAL_CHUNKS = 62500;   // Total chunks in the world (1,000,000 / 16)
    
    int worldHeight;                         // Height of the world in blocks
    int tileSize;                            // Size of a tile in pixels
    uint64_t currentSeed;                    // Current world seed
    
    // Perlin noise generator for terrain
    PerlinNoise terrainNoise;                // For terrain height
    
    // Tile manager
    TileManager tileManager;
    
    // Map of active chunks (key is chunk X position)
    std::map<int, Chunk*> activeChunks;
    
    void updateActiveChunks(int centerChunkX);
    
public:
    World(int worldHeight, int tileSize, uint64_t seed);
    ~World();
    
    void reset(uint64_t seed);
    void draw(sf::RenderWindow& window);
    void update(float viewCenterX);
    
    // Get dimensions for camera boundaries
    int getWorldWidth() const { return TOTAL_CHUNKS * CHUNK_WIDTH * tileSize; }
    int getWorldHeight() const { return worldHeight * tileSize; }
}; 