#include "TileManager.h"

TileManager::TileManager(const std::string& path) : texturePath(path) {
    initializeTileFilenames();
}

void TileManager::initializeTileFilenames() {
    // Map tile types to their respective filenames
    tileFilenames[TileType::GRASS] = "dirt_grass.png";
    tileFilenames[TileType::DIRT] = "dirt.png";
    tileFilenames[TileType::STONE] = "stone.png";
    tileFilenames[TileType::BEDROCK] = "stone_coal.png"; // Using coal stone as bedrock
    tileFilenames[TileType::TRUNK] = "trunk_side.png";
    tileFilenames[TileType::LEAVES] = "leaves_transparent.png";
    tileFilenames[TileType::SNOW_GRASS] = "dirt_snow.png";
    tileFilenames[TileType::SNOW] = "snow.png";
    tileFilenames[TileType::COAL_ORE] = "stone_coal.png";
    tileFilenames[TileType::IRON_ORE] = "stone_silver.png"; // Using silver for iron
    tileFilenames[TileType::GOLD_ORE] = "stone_gold.png";
    tileFilenames[TileType::DIAMOND_ORE] = "stone_diamond.png";
    tileFilenames[TileType::GRAVELED_STONE] = "gravel_stone.png";
}

bool TileManager::loadTextures() {
    bool success = true;
    
    // Clear any existing textures
    tileTextures.clear();
    
    // Load all textures
    for (const auto& pair : tileFilenames) {
        TileType type = pair.first;
        std::string filename = pair.second;
        
        if (!tileTextures[type].loadFromFile(texturePath + filename)) {
            std::cerr << "Failed to load texture: " << texturePath << filename << std::endl;
            success = false;
        } else {
            // Disable texture smoothing for pixel art
            tileTextures[type].setSmooth(false);
        }
    }
    
    return success;
}

sf::Texture* TileManager::getTexture(TileType type) {
    if (tileTextures.find(type) != tileTextures.end()) {
        return &tileTextures[type];
    }
    return nullptr;
}

void TileManager::setTexturePath(const std::string& path) {
    texturePath = path;
    // Reload textures with new path
    loadTextures();
}

bool TileManager::areTexturesLoaded() const {
    // Check if all expected textures are loaded
    for (const auto& pair : tileFilenames) {
        if (tileTextures.find(pair.first) == tileTextures.end()) {
            return false;
        }
    }
    return true;
} 