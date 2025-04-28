#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "PerlinNoise.h"
#include "TileTypes.h"

class Chunk {
private:
    int chunkX;        // Chunk X position in world (chunk index)
    int chunkWidth;    // Width of chunk (16 blocks)
    int worldHeight;   // Height of chunk (same as world height)
    int tileSize;      // Size of a tile in pixels
    bool isGenerated;  // Whether this chunk has been generated
    
    std::vector<std::vector<TileType>> tiles; // 2D array of tiles in this chunk
    std::vector<sf::Sprite> sprites;          // Sprites for rendering
    
    sf::Texture* grassTexture;
    sf::Texture* dirtTexture;
    sf::Texture* stoneTexture;
    sf::Texture* graveledStoneTexture;
    sf::Texture* trunkTexture;
    sf::Texture* leavesTexture;
    
    void generateTerrain(PerlinNoise& terrainNoise, uint64_t seed, int worldOffset);
    void generateTrees(uint64_t seed, int worldOffset);
    void buildSpriteArray();

public:
    Chunk(int x, int width, int height, int tileSize, 
          sf::Texture* grass, sf::Texture* dirt, sf::Texture* stone, 
          sf::Texture* graveledStone, sf::Texture* trunk, sf::Texture* leaves);
    
    void generate(PerlinNoise& terrainNoise, uint64_t seed, int worldOffset);
    void draw(sf::RenderWindow& window);
    
    int getChunkX() const { return chunkX; }
    int getWorldX() const { return chunkX * chunkWidth; }
    int getWidth() const { return chunkWidth; }
    bool isActive() const { return isGenerated; }
    void unload() { sprites.clear(); isGenerated = false; }
}; 