#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include "TileTypes.h"

class TileManager {
private:
    // Store textures by tile type
    std::unordered_map<TileType, sf::Texture> tileTextures;
    
    // Path to texture directory
    std::string texturePath;
    
    // Map of tile types to their file names
    std::unordered_map<TileType, std::string> tileFilenames;
    
    // Initialize the tile filename map
    void initializeTileFilenames();

public:
    // Constructor
    TileManager(const std::string& path = "assets/textures/");
    
    // Load all textures
    bool loadTextures();
    
    // Get texture for a specific tile type
    sf::Texture* getTexture(TileType type);
    
    // Set a new texture path
    void setTexturePath(const std::string& path);
    
    // Check if all textures are loaded
    bool areTexturesLoaded() const;
}; 