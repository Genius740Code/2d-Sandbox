#include "World.h"
#include <algorithm>
#include <cmath>

World::World(int height, int tileSize, uint64_t seed) : 
    worldHeight(height),
    tileSize(tileSize),
    currentSeed(seed),
    terrainNoise(seed)
{
    initializeTextures();
    
    // Initialize with a completely empty world
    // Chunks will be generated on demand when update() is called
}

World::~World() {
    // Clean up all active chunks
    for (auto& pair : activeChunks) {
        delete pair.second;
    }
    activeChunks.clear();
}

void World::reset(uint64_t seed) {
    // Delete all existing chunks
    for (auto& pair : activeChunks) {
        delete pair.second;
    }
    activeChunks.clear();
    
    // Set new seed
    currentSeed = seed;
    terrainNoise = PerlinNoise(seed);
    
    // Chunks will be regenerated on next update
}

void World::initializeTextures() {
    // Load textures
    if (!grassTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/dirt_grass.png")) {
        std::cerr << "Failed to load grass texture!" << std::endl;
    }
    if (!dirtTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/dirt.png")) {
        std::cerr << "Failed to load dirt texture!" << std::endl;
    }
    if (!stoneTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/stone.png")) {
        std::cerr << "Failed to load stone texture!" << std::endl;
    }
    if (!trunkTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/trunk_side.png")) {
        std::cerr << "Failed to load trunk texture!" << std::endl;
    }
    if (!leavesTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/leaves_transparent.png")) {
        std::cerr << "Failed to load leaves texture!" << std::endl;
    }
    
    // Disable texture smoothing
    grassTexture.setSmooth(false);
    dirtTexture.setSmooth(false);
    stoneTexture.setSmooth(false);
    trunkTexture.setSmooth(false);
    leavesTexture.setSmooth(false);
}

void World::updateActiveChunks(int centerChunkX) {
    // Calculate range of chunks to keep active (centered on the player's view)
    int minChunk = std::max(0, centerChunkX - MAX_CHUNKS / 2);
    int maxChunk = std::min(TOTAL_CHUNKS - 1, centerChunkX + MAX_CHUNKS / 2);
    
    // Remove chunks that are now too far away
    std::vector<int> chunksToRemove;
    for (auto& pair : activeChunks) {
        int chunkX = pair.first;
        if (chunkX < minChunk || chunkX > maxChunk) {
            chunksToRemove.push_back(chunkX);
        }
    }
    
    for (int chunkX : chunksToRemove) {
        delete activeChunks[chunkX];
        activeChunks.erase(chunkX);
    }
    
    // Add new chunks that are now in range
    for (int chunkX = minChunk; chunkX <= maxChunk; chunkX++) {
        if (activeChunks.find(chunkX) == activeChunks.end()) {
            // Create a new chunk
            Chunk* newChunk = new Chunk(
                chunkX, 
                CHUNK_WIDTH, 
                worldHeight, 
                tileSize,
                &grassTexture, 
                &dirtTexture, 
                &stoneTexture, 
                &trunkTexture, 
                &leavesTexture
            );
            
            // Generate terrain for this chunk
            newChunk->generate(terrainNoise, currentSeed, chunkX * CHUNK_WIDTH);
            
            // Add to active chunks
            activeChunks[chunkX] = newChunk;
        }
    }
}

void World::update(float viewCenterX) {
    // Calculate which chunk the view is centered on
    int centerChunkX = static_cast<int>(viewCenterX) / (CHUNK_WIDTH * tileSize);
    
    // Update which chunks are active
    updateActiveChunks(centerChunkX);
}

void World::draw(sf::RenderWindow& window) {
    // Draw all active chunks
    for (auto& pair : activeChunks) {
        pair.second->draw(window);
    }
} 