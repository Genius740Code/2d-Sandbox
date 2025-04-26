#include "World.h"
#include <random>

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
    
    // Generate trees
    generateTrees(seed);
    
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
    
    // Generate new trees
    generateTrees(seed);
    
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
    if (!trunkTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/trunk_side.png")) {
        std::cerr << "Failed to load trunk texture!" << std::endl;
    }
    if (!leavesTexture.loadFromFile("kenney_voxel-pack/PNG/Tiles/leaves_transparent.png")) {
        std::cerr << "Failed to load leaves texture!" << std::endl;
    }
    
    // Scale textures to tile size
    grassTexture.setSmooth(false);
    dirtTexture.setSmooth(false);
    stoneTexture.setSmooth(false);
    trunkTexture.setSmooth(false);
    leavesTexture.setSmooth(false);
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

void World::generateTrees(uint64_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> treeDist(0, 100); // Probability of tree generation
    std::uniform_int_distribution<int> heightDist(4, 6); // Tree height variation (4-6 blocks tall)
    
    // Find suitable positions for trees (on grass blocks)
    for (int x = 6; x < width - 6; x++) {
        // Only place a tree if random chance is met (about 8%)
        if (treeDist(rng) > 92) {
            // Find the ground level at this x position
            for (int y = 0; y < height; y++) {
                if (tiles[x][y] == GRASS) {
                    // Place a tree at this position if there's enough room above
                    int treeHeight = heightDist(rng);
                    if (y - treeHeight > 4) { // Ensure enough space for trunk and leaves
                        // Place trunk sections (vertical column)
                        for (int i = 1; i <= treeHeight; i++) {
                            tiles[x][y-i] = TRUNK;
                        }
                        
                        // The top position of the trunk
                        int topY = y - treeHeight;
                        
                        // Minecraft-style tree leaf pattern
                        // Layer 1 (bottom) - wider layer
                        for (int lx = x - 2; lx <= x + 2; lx++) {
                            for (int ly = topY - 1; ly <= topY; ly++) {
                                if (lx < 0 || lx >= width || ly < 0 || ly >= height) continue;
                                
                                // Skip some corner blocks for more natural shape
                                if ((lx == x - 2 || lx == x + 2) && treeDist(rng) < 40) continue;
                                
                                if (tiles[lx][ly] == AIR) {
                                    tiles[lx][ly] = LEAVES;
                                }
                            }
                        }
                        
                        // Layer 2 (middle) - full layer
                        for (int lx = x - 2; lx <= x + 2; lx++) {
                            if (lx < 0 || lx >= width) continue;
                            
                            int ly = topY - 2;
                            if (ly < 0 || ly >= height) continue;
                            
                            // Make corners a bit more sparse
                            if ((lx == x - 2 || lx == x + 2) && (treeDist(rng) < 30)) continue;
                            
                            if (tiles[lx][ly] == AIR) {
                                tiles[lx][ly] = LEAVES;
                            }
                        }
                        
                        // Layer 3 (top) - smaller layer
                        for (int lx = x - 1; lx <= x + 1; lx++) {
                            if (lx < 0 || lx >= width) continue;
                            
                            int ly = topY - 3;
                            if (ly < 0 || ly >= height) continue;
                            
                            if (tiles[lx][ly] == AIR) {
                                tiles[lx][ly] = LEAVES;
                            }
                        }
                        
                        // Top leaf
                        if (topY - 4 >= 0) {
                            tiles[x][topY - 4] = LEAVES;
                        }
                    }
                    break; // Stop after finding the ground level
                }
            }
        }
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
                    case TRUNK:
                        sprite.setTexture(trunkTexture);
                        break;
                    case LEAVES:
                        sprite.setTexture(leavesTexture);
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