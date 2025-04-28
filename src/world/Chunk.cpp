#include "Chunk.h"
#include <random>

Chunk::Chunk(int x, int width, int height, int tileSize, 
             sf::Texture* grass, sf::Texture* dirt, sf::Texture* stone, 
             sf::Texture* graveledStone, sf::Texture* trunk, sf::Texture* leaves) :
    chunkX(x),
    chunkWidth(width),
    worldHeight(height),
    tileSize(tileSize),
    isGenerated(false),
    grassTexture(grass),
    dirtTexture(dirt),
    stoneTexture(stone),
    graveledStoneTexture(graveledStone),
    trunkTexture(trunk),
    leavesTexture(leaves) {
    
    // Initialize the chunk with air
    tiles.resize(chunkWidth, std::vector<TileType>(worldHeight, AIR));
}

void Chunk::generate(PerlinNoise& terrainNoise, uint64_t seed, int worldOffset) {
    // Generate terrain and trees for this chunk
    generateTerrain(terrainNoise, seed, worldOffset);
    generateTrees(seed, worldOffset);
    buildSpriteArray();
    isGenerated = true;
}

void Chunk::generateTerrain(PerlinNoise& terrainNoise, uint64_t seed, int worldOffset) {
    // Parameters for terrain generation
    const double scale = 0.05;
    const int dirtLayers = 3;
    const int baseHeight = worldHeight * 0.5;
    const int hillHeight = worldHeight * 0.18;
    
    // Setup random number generator for graveled stone distribution
    std::mt19937 rng(seed + chunkX);
    std::uniform_int_distribution<int> stoneDist(0, 1); // 50% chance for graveled stone
    
    // Generate the base terrain heightmap for this chunk
    for (int x = 0; x < chunkWidth; x++) {
        // Calculate the world x-coordinate
        int worldX = worldOffset + x;
        
        // Use perlin noise to generate the height at this position
        double heightValue = terrainNoise.noise(worldX * scale, 0) * 0.5 + 0.5;
        int terrainHeight = baseHeight - hillHeight * heightValue;
        
        if (terrainHeight >= 0 && terrainHeight < worldHeight) {
            tiles[x][terrainHeight] = GRASS;
            
            for (int dirt = 1; dirt <= dirtLayers; dirt++) {
                int y = terrainHeight + dirt;
                if (y < worldHeight) {
                    tiles[x][y] = DIRT;
                }
            }
            
            for (int y = terrainHeight + dirtLayers + 1; y < worldHeight; y++) {
                if (stoneDist(rng) == 0) {
                    tiles[x][y] = STONE;
                } else {
                    tiles[x][y] = GRAVELED_STONE;
                }
            }
        }
    }
    
    // Add bedrock at the bottom
    for (int x = 0; x < chunkWidth; x++) {
        tiles[x][worldHeight - 1] = BEDROCK;
    }
}

void Chunk::generateTrees(uint64_t seed, int /*worldOffset*/) {
    std::mt19937 rng(seed + chunkX); // Use chunk position to vary the seed
    std::uniform_int_distribution<int> treeDist(0, 100); // Probability of tree generation
    std::uniform_int_distribution<int> heightDist(4, 6); // Tree height variation (4-6 blocks tall)
    
    // Find suitable positions for trees (on grass blocks)
    // Don't place trees at the chunk edges to avoid issues with leaves crossing chunks
    for (int x = 2; x < chunkWidth - 2; x++) {
        // Only place a tree if random chance is met (about 8%)
        if (treeDist(rng) > 92) {
            // Find the ground level at this x position
            for (int y = 0; y < worldHeight; y++) {
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
                                if (lx < 0 || lx >= chunkWidth || ly < 0 || ly >= worldHeight) continue;
                                
                                // Skip some corner blocks for more natural shape
                                if ((lx == x - 2 || lx == x + 2) && treeDist(rng) < 40) continue;
                                
                                if (tiles[lx][ly] == AIR) {
                                    tiles[lx][ly] = LEAVES;
                                }
                            }
                        }
                        
                        // Layer 2 (middle) - full layer
                        for (int lx = x - 2; lx <= x + 2; lx++) {
                            if (lx < 0 || lx >= chunkWidth) continue;
                            
                            int ly = topY - 2;
                            if (ly < 0 || ly >= worldHeight) continue;
                            
                            // Make corners a bit more sparse
                            if ((lx == x - 2 || lx == x + 2) && (treeDist(rng) < 30)) continue;
                            
                            if (tiles[lx][ly] == AIR) {
                                tiles[lx][ly] = LEAVES;
                            }
                        }
                        
                        // Layer 3 (top) - smaller layer
                        for (int lx = x - 1; lx <= x + 1; lx++) {
                            if (lx < 0 || lx >= chunkWidth) continue;
                            
                            int ly = topY - 3;
                            if (ly < 0 || ly >= worldHeight) continue;
                            
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

void Chunk::buildSpriteArray() {
    sprites.clear();
    sprites.reserve(chunkWidth * worldHeight); // Reserve space for efficiency
    
    // Calculate the world X position of this chunk in pixels
    int worldPosX = chunkX * chunkWidth * tileSize;
    
    // Create a sprite for each tile
    for (int x = 0; x < chunkWidth; x++) {
        for (int y = 0; y < worldHeight; y++) {
            if (tiles[x][y] != AIR) {
                sf::Sprite sprite;
                
                switch (tiles[x][y]) {
                    case GRASS:
                        sprite.setTexture(*grassTexture);
                        break;
                    case DIRT:
                        sprite.setTexture(*dirtTexture);
                        break;
                    case STONE:
                        sprite.setTexture(*stoneTexture);
                        break;
                    case GRAVELED_STONE:
                        sprite.setTexture(*graveledStoneTexture);
                        break;
                    case BEDROCK:
                        sprite.setTexture(*stoneTexture);
                        break;
                    case TRUNK:
                        sprite.setTexture(*trunkTexture);
                        break;
                    case LEAVES:
                        sprite.setTexture(*leavesTexture);
                        break;
                    default:
                        continue; // Skip air tiles
                }
                
                // Scale sprite to match tile size
                float scaleX = static_cast<float>(tileSize) / sprite.getTexture()->getSize().x;
                float scaleY = static_cast<float>(tileSize) / sprite.getTexture()->getSize().y;
                sprite.setScale(scaleX, scaleY);
                
                // Position the sprite in the world
                sprite.setPosition(worldPosX + x * tileSize, y * tileSize);
                
                sprites.push_back(sprite);
            }
        }
    }
}

void Chunk::draw(sf::RenderWindow& window) {
    if (!isGenerated) return;
    
    // Get the view to determine what's visible
    sf::View view = window.getView();
    float viewLeft = view.getCenter().x - view.getSize().x / 2;
    float viewRight = view.getCenter().x + view.getSize().x / 2;
    
    // Simple frustum culling - only draw if chunk is in view
    int chunkWorldPosX = chunkX * chunkWidth * tileSize;
    int chunkWorldPosRight = chunkWorldPosX + chunkWidth * tileSize;
    
    if (chunkWorldPosRight < viewLeft || chunkWorldPosX > viewRight) {
        return; // Chunk is not visible
    }
    
    // Draw all sprites in the chunk
    for (const auto& sprite : sprites) {
        window.draw(sprite);
    }
} 