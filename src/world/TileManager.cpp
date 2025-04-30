#include "TileManager.h"
#include <fstream>

TileManager::TileManager(const std::string& path) : texturePath(path) {
    std::cout << "Initializing TileManager with path: " << path << std::endl;
    initializeTileFilenames();
}

void TileManager::initializeTileFilenames() {
    // Map tile types to their respective filenames
    tileFilenames[TileType::GRASS] = "dirt_grass.png";
    tileFilenames[TileType::DIRT] = "dirt.png";
    tileFilenames[TileType::STONE] = "stone.png";
    tileFilenames[TileType::BEDROCK] = "stone_coal.png"; // Using coal stone as bedrock
    tileFilenames[TileType::TRUNK] = "oak_wood.png";
    tileFilenames[TileType::LEAVES] = "leaves_transparent.png";
    tileFilenames[TileType::SNOW_GRASS] = "dirt_snow.png";
    tileFilenames[TileType::SNOW] = "snow.png";
    tileFilenames[TileType::COAL_ORE] = "stone_coal.png";
    tileFilenames[TileType::IRON_ORE] = "stone_silver.png"; // Using silver for iron
    tileFilenames[TileType::GOLD_ORE] = "stone_gold.png";
    tileFilenames[TileType::DIAMOND_ORE] = "stone_diamond.png";
    tileFilenames[TileType::GRAVELED_STONE] = "gravel_stone.png";
    tileFilenames[TileType::BRICK] = "brick_grey.png";
    tileFilenames[TileType::WATER] = "water.png";
    tileFilenames[TileType::SAND] = "sand.png"; // Now using actual sand.png
    tileFilenames[TileType::LAVA] = "lava.png";
}

bool TileManager::loadTextures() {
    bool success = true;
    int loadedCount = 0;
    int failedCount = 0;
    
    std::cout << "Loading textures from: " << texturePath << std::endl;
    
    // Check if directory exists
    std::ifstream testFile(texturePath + "dirt.png");
    if (!testFile.good()) {
        std::cerr << "ERROR: Texture directory might not exist or is inaccessible!" << std::endl;
        std::cerr << "Current working directory: ";
        system("cd");
        std::cerr << "Checking texture directory content:" << std::endl;
        system(("dir " + texturePath).c_str());
    }
    testFile.close();
    
    // Clear any existing textures
    tileTextures.clear();
    
    // Load all textures
    for (const auto& pair : tileFilenames) {
        TileType type = pair.first;
        std::string filename = pair.second;
        std::string fullPath = texturePath + filename;
        
        // Try loading from base directory only (since we flattened the structure)
        if (!tileTextures[type].loadFromFile(fullPath)) {
            std::cerr << "Failed to load texture: " << fullPath << std::endl;
            success = false;
            failedCount++;
            
            // Try one level up as fallback
            std::string fallbackPath = "../" + texturePath + filename;
            if (tileTextures[type].loadFromFile(fallbackPath)) {
                std::cout << "Successfully loaded from fallback path: " << fallbackPath << std::endl;
                tileTextures[type].setSmooth(false);
                loadedCount++;
                continue;
            }
        } else {
            // Disable texture smoothing for pixel art
            tileTextures[type].setSmooth(false);
            loadedCount++;
        }
    }
    
    std::cout << "Texture loading summary: " << loadedCount << " loaded, " 
              << failedCount << " failed" << std::endl;
    
    return success;
}

sf::Texture* TileManager::getTexture(TileType type) {
    if (tileTextures.find(type) != tileTextures.end()) {
        return &tileTextures[type];
    }
    std::cerr << "Warning: Attempted to get non-existent texture for type: " << static_cast<int>(type) << std::endl;
    return nullptr;
}

void TileManager::setTexturePath(const std::string& path) {
    std::cout << "Changing texture path from: " << texturePath << " to: " << path << std::endl;
    texturePath = path;
    // Reload textures with new path
    loadTextures();
}

bool TileManager::areTexturesLoaded() const {
    // Check if all expected textures are loaded
    int count = 0;
    for (const auto& pair : tileFilenames) {
        if (tileTextures.find(pair.first) == tileTextures.end()) {
            return false;
        }
        count++;
    }
    std::cout << "All " << count << " textures are loaded." << std::endl;
    return true;
} 