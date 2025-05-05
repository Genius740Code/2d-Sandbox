#include "World.h"
#include <algorithm>
#include <cmath>
#include <chrono>

World::World(int height, int tileSize, uint64_t seed) : 
    worldHeight(height),
    tileSize(tileSize),
    currentSeed(seed),
    terrainNoise(seed),
    tileManager("assets/textures/")
{
    // Load textures
    auto startTime = std::chrono::high_resolution_clock::now();
    if (!tileManager.loadTextures()) {
        std::cerr << "Failed to load one or more textures!" << std::endl;
    } else {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Textures loaded successfully in " << duration.count() << "ms" << std::endl;
    }
    
    // Initialize with a completely empty world
    // Chunks will be generated on demand when update() is called
}

World::~World() {
    // With smart pointers, manual cleanup is no longer needed
    // Chunks will be automatically deleted when the map is cleared
    activeChunks.clear();
}

void World::reset(uint64_t seed) {
    // Clear all existing chunks
    activeChunks.clear();
    
    // Set new seed
    currentSeed = seed;
    terrainNoise = PerlinNoise(seed);
    
    // Chunks will be regenerated on next update
}

void World::update(float viewCenterX) {
    // Calculate the center chunk
    int centerChunkX = static_cast<int>(viewCenterX / (CHUNK_WIDTH * tileSize));
    
    // Update active chunks based on new center
    updateActiveChunks(centerChunkX);
}

void World::updateActiveChunks(int centerChunkX) {
    // Performance optimization: Track which chunks need to be generated
    std::vector<int> chunksToGenerate;
    
    // First, mark chunks outside view distance for removal
    std::vector<int> chunksToRemove;
    for (auto& pair : activeChunks) {
        int chunkX = pair.first;
        if (abs(chunkX - centerChunkX) > MAX_CHUNKS / 2) {
            chunksToRemove.push_back(chunkX);
        }
    }
    
    // Remove chunks outside view distance
    for (int chunkX : chunksToRemove) {
        activeChunks.erase(chunkX);
    }
    
    // Calculate the range of chunks to keep active (centered around the player)
    int startChunkX = centerChunkX - MAX_CHUNKS / 2;
    int endChunkX = centerChunkX + MAX_CHUNKS / 2;
    
    // Make sure chunks in view range are active
    for (int x = startChunkX; x <= endChunkX; x++) {
        // Skip if chunk is already active
        if (activeChunks.find(x) != activeChunks.end()) {
            continue;
        }
        
        // Create chunk object and generate terrain
        auto chunk = std::make_unique<Chunk>(
            x, CHUNK_WIDTH, worldHeight, tileSize,
            tileManager.getTexture(TileType::GRASS),
            tileManager.getTexture(TileType::DIRT),
            tileManager.getTexture(TileType::STONE),
            tileManager.getTexture(TileType::GRAVELED_STONE),
            tileManager.getTexture(TileType::TRUNK),
            tileManager.getTexture(TileType::LEAVES)
        );
        
        // Queue for generation
        chunksToGenerate.push_back(x);
        activeChunks[x] = std::move(chunk);
    }
    
    // Generate chunks in a separate phase to allow for multithreading in future
    for (int x : chunksToGenerate) {
        auto it = activeChunks.find(x);
        if (it != activeChunks.end()) {
            // Calculate the world offset for this chunk
            int worldOffset = x * CHUNK_WIDTH;
            it->second->generate(terrainNoise, currentSeed, worldOffset);
        }
    }
}

void World::draw(sf::RenderWindow& window) {
    // Draw all active chunks
    for (auto& pair : activeChunks) {
        pair.second->draw(window);
    }
} 