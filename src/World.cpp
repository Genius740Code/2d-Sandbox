#include "World.h"

World::World(int w, int h, int tileSize, uint64_t seed) : 
    width(w), 
    height(h), 
    tileSize(tileSize)
{
    initializeTextures();
    
    // Initialize the world with air
    tiles.resize(width, std::vector<TileType>(height, AIR));
    
    // Generate terrain
    generateTerrain(seed);
    
    // Build sprite array for faster rendering
    buildSpriteArray();
}

void World::reset(uint64_t seed) {
    // Clear existing data
    tiles.clear();
    sprites.clear();
    
    // Reinitialize the world with air
    tiles.resize(width, std::vector<TileType>(height, AIR));
    
    // Generate new terrain
    generateTerrain(seed);
    
    // Rebuild sprite array
    buildSpriteArray();
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
    
    // Scale textures to tile size
    grassTexture.setSmooth(false);
    dirtTexture.setSmooth(false);
    stoneTexture.setSmooth(false);
}

void World::generateTerrain(uint64_t seed) {
    PerlinNoise terrain(seed);
    
    // Parameters for terrain generation
    const double scale = 0.05;
    const int dirtLayers = 3;
    const int baseHeight = height * 0.5;
    const int hillHeight = height * 0.18;
    
    // Generate the base terrain heightmap
    std::vector<int> heightMap(width);
    for (int x = 0; x < width; x++) {
        double heightValue = terrain.noise(x * scale, 0) * 0.5 + 0.5;
        int terrainHeight = baseHeight - hillHeight * heightValue;
        heightMap[x] = terrainHeight;
        
        if (terrainHeight >= 0 && terrainHeight < height) {
            tiles[x][terrainHeight] = GRASS;
            
            for (int dirt = 1; dirt <= dirtLayers; dirt++) {
                int y = terrainHeight + dirt;
                if (y < height) {
                    tiles[x][y] = DIRT;
                }
            }
            
            for (int y = terrainHeight + dirtLayers + 1; y < height; y++) {
                tiles[x][y] = STONE;
            }
        }
    }
    
    // Add bedrock at the bottom
    for (int x = 0; x < width; x++) {
        tiles[x][height - 1] = BEDROCK;
    }
}

void World::buildSpriteArray() {
    sprites.clear();
    sprites.reserve(width * height);
    
    // Create a sprite for each tile
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (tiles[x][y] != AIR) {
                sf::Sprite sprite;
                
                switch (tiles[x][y]) {
                    case GRASS:
                        sprite.setTexture(grassTexture);
                        break;
                    case DIRT:
                        sprite.setTexture(dirtTexture);
                        break;
                    case STONE:
                    case BEDROCK:
                        sprite.setTexture(stoneTexture);
                        break;
                    default:
                        continue; // Skip air tiles
                }
                
                // Scale sprite to match tile size
                float scaleX = static_cast<float>(tileSize) / sprite.getTexture()->getSize().x;
                float scaleY = static_cast<float>(tileSize) / sprite.getTexture()->getSize().y;
                sprite.setScale(scaleX, scaleY);
                
                // Position the sprite
                sprite.setPosition(x * tileSize, y * tileSize);
                
                sprites.push_back(sprite);
            }
        }
    }
}

void World::draw(sf::RenderWindow& window) {
    for (const auto& sprite : sprites) {
        window.draw(sprite);
    }
} 