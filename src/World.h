#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdint>
#include "PerlinNoise.h"

// Tile types for our 2D Minecraft-like terrain
enum TileType {
    AIR,
    GRASS,
    DIRT,
    STONE,
    BEDROCK
};

class World {
private:
    int width, height;
    int tileSize;
    std::vector<std::vector<TileType>> tiles;
    
    sf::Texture grassTexture;
    sf::Texture dirtTexture;
    sf::Texture stoneTexture;
    
    // To store the sprites for faster rendering
    std::vector<sf::Sprite> sprites;
    
    // Cave generation parameters removed
    
    void initializeTextures();
    void buildSpriteArray();
    void generateTerrain(uint64_t seed);
    // Cave generation methods removed

public:
    World(int w, int h, int tileSize, uint64_t seed);
    void reset(uint64_t seed);
    void draw(sf::RenderWindow& window);
    
    // Get dimensions for camera boundaries
    int getWorldWidth() const { return width * tileSize; }
    int getWorldHeight() const { return height * tileSize; }
}; 