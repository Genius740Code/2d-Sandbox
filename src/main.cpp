#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>  // Added for string formatting

#include "PerlinNoise.h"
#include "World.h"
#include "Camera.h"
#include "ui/MenuState.h"

int main() {
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Minecraft");
    window.setFramerateLimit(60);
    
    // World parameters
    const int worldHeight = 200;
    const int tileSize = 16;
    
    // Game state tracking
    GameState currentState = GameState::MAIN_MENU;
    GameMode gameMode = GameMode::SURVIVAL;
    Difficulty difficulty = Difficulty::NORMAL;
    std::string worldName = "New World";
    
    // Create menu system
    MenuState menuState;
    if (!menuState.initialize()) {
        std::cerr << "Failed to initialize menu system!" << std::endl;
        return 1;
    }
    
    // Set up menu callbacks
    menuState.setOnStateChange([&currentState](GameState newState) {
        currentState = newState;
    });
    
    menuState.setOnCreateWorld([&worldName, &gameMode, &difficulty](
        const std::string& name, GameMode mode, Difficulty diff) {
        worldName = name;
        gameMode = mode;
        difficulty = diff;
    });
    
    // Create a big seed value
    uint64_t seedMin = static_cast<uint64_t>(1) << 50;
    uint64_t seedMax = static_cast<uint64_t>(1) << 60;
    
    // Random seed generation
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(seedMin, seedMax);
    uint64_t seed = dis(gen);
    
    // Create the world
    World world(worldHeight, tileSize, seed);
    
    // Create camera
    Camera camera(windowWidth, windowHeight, world.getWorldWidth(), world.getWorldHeight());
    camera.setCreativeMode(gameMode == GameMode::CREATIVE);
    
    // Font for in-game UI
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    
    sf::Text chunkText;
    chunkText.setFont(font);
    chunkText.setCharacterSize(14);
    chunkText.setFillColor(sf::Color::White);
    chunkText.setOutlineColor(sf::Color::Black);
    chunkText.setOutlineThickness(1);
    
    // FPS counter text
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(14);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(1);
    fpsText.setPosition(10, 30);
    
    // Game info text (displays world name, game mode, difficulty)
    sf::Text gameInfoText;
    gameInfoText.setFont(font);
    gameInfoText.setCharacterSize(14);
    gameInfoText.setFillColor(sf::Color::White);
    gameInfoText.setOutlineColor(sf::Color::Black);
    gameInfoText.setOutlineThickness(1);
    gameInfoText.setPosition(10, 50);
    
    // Main game loop
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fpsUpdateTime = 0.0f;
    float fps = 0.0f;
    
    while (window.isOpen()) {
        // Time delta for smooth movement
        float dt = clock.restart().asSeconds();
        
        // Update FPS counter
        frameCount++;
        fpsUpdateTime += dt;
        if (fpsUpdateTime >= 0.5f) {
            fps = frameCount / fpsUpdateTime;
            frameCount = 0;
            fpsUpdateTime = 0.0f;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
        }
        
        // Get mouse position
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (currentState == GameState::GAME) {
                        // Return to main menu
                        currentState = GameState::MAIN_MENU;
                        menuState.setupMainMenu();
                    } else if (currentState == GameState::MAIN_MENU) {
                        window.close();
                    }
                }
                
                // Game-specific controls
                if (currentState == GameState::GAME) {
                    // Generate new world on space press
                    if (event.key.code == sf::Keyboard::Space) {
                        seed = dis(gen);
                        std::cout << "Generated new world with seed: " << seed << std::endl;
                        world.reset(seed);
                        camera.reset();
                    }
                    
                    // Toggle creative mode (only if not in hardcore)
                    if (event.key.code == sf::Keyboard::C && gameMode != GameMode::HARDCORE) {
                        bool isCreative = (gameMode == GameMode::CREATIVE);
                        gameMode = isCreative ? GameMode::SURVIVAL : GameMode::CREATIVE;
                        camera.setCreativeMode(gameMode == GameMode::CREATIVE);
                    }
                    
                    // Zoom controls
                    if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
                        camera.zoom(0.9f);
                    if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash)
                        camera.zoom(1.1f);
                    if (event.key.code == sf::Keyboard::Num0)
                        camera.reset();
                }
            }
            
            // Pass events to menu if in menu state
            if (currentState != GameState::GAME) {
                menuState.handleEvent(event, window);
            }
        }
        
        // Update based on current state
        if (currentState == GameState::GAME) {
            // Handle keyboard input for movement
            float dx = 0.0f, dy = 0.0f;
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                dx -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                dx += 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                dy -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                dy += 1.0f;
                
            // Move camera if input detected
            if (dx != 0.0f || dy != 0.0f) {
                // Normalize for diagonal movement
                if (dx != 0.0f && dy != 0.0f) {
                    dx *= 0.7071f;
                    dy *= 0.7071f;
                }
                
                camera.move(dx, dy, dt);
            }
            
            // Get the current camera view
            const sf::View& view = camera.getView();
            float centerX = view.getCenter().x;
            
            // Update the world (load/unload chunks)
            world.update(centerX);
            
            // Update chunk information text
            int currentChunk = static_cast<int>(centerX) / (16 * tileSize);
            
            std::string chunkInfo = "Chunk: " + std::to_string(currentChunk) + 
                                  " / 62499 | Position: " + std::to_string(static_cast<int>(centerX)) + 
                                  " / " + std::to_string(world.getWorldWidth());
            chunkText.setString(chunkInfo);
            
            // Update game info text
            std::string modeStr = (gameMode == GameMode::SURVIVAL) ? "Survival" : 
                               (gameMode == GameMode::CREATIVE) ? "Creative" : "Hardcore";
            std::string diffStr = (difficulty == Difficulty::PEACEFUL) ? "Peaceful" :
                              (difficulty == Difficulty::EASY) ? "Easy" :
                              (difficulty == Difficulty::NORMAL) ? "Normal" : "Hard";
            gameInfoText.setString("World: " + worldName + " | Mode: " + modeStr + " | Difficulty: " + diffStr);
            
            // Update the view
            window.setView(view);
            
            // Clear the window
            window.clear(sf::Color(135, 206, 235));
            
            // Draw the world
            world.draw(window);
            
            // Draw UI elements with fixed position relative to the view
            sf::View prevView = window.getView();
            sf::View uiView = window.getDefaultView();
            window.setView(uiView);
            
            // Position the text in the top-left corner
            chunkText.setPosition(10, 10);
            window.draw(chunkText);
            window.draw(fpsText);
            window.draw(gameInfoText);
            
            // Restore the game view
            window.setView(prevView);
        }
        else if (currentState == GameState::EXIT) {
            window.close();
        }
        else {
            // Menu states
            window.clear();
            menuState.update(mousePos);
            menuState.draw(window);
        }
        
        // Display the window
        window.display();
    }
    
    return 0;
} 