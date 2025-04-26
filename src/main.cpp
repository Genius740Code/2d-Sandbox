#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <cstdint>
#include <string>

#include "PerlinNoise.h"
#include "World.h"
#include "Camera.h"

int main() {
    // Create window
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Minecraft Chunked World Generator");
    window.setFramerateLimit(60);
    
    // World parameters
    const int worldHeight = 200;
    const int tileSize = 16;
    
    // Creative mode flag
    bool creativeMode = true;
    
    // Create a big seed value as requested
    uint64_t seedMin = static_cast<uint64_t>(1) << 50; // 2^50
    uint64_t seedMax = static_cast<uint64_t>(1) << 60; // 2^60
    
    // Random seed generation
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(seedMin, seedMax);
    uint64_t seed = dis(gen);
    
    // Print the seed and controls
    std::cout << "Generated world with seed: " << seed << std::endl;
    std::cout << "The world is now 1,000,000 blocks wide with 16-block chunks!" << std::endl;
    std::cout << "Only 7 chunks are active at a time to optimize performance." << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD/Arrows: Move camera" << std::endl;
    std::cout << "  C: Toggle creative mode (affects movement speed)" << std::endl;
    std::cout << "  +/-: Zoom in/out" << std::endl;
    std::cout << "  0: Reset view" << std::endl;
    std::cout << "  Space: Generate new world" << std::endl;
    std::cout << "  Esc: Exit" << std::endl;
    
    // Create the world
    World world(worldHeight, tileSize, seed);
    
    // Create camera
    Camera camera(windowWidth, windowHeight, world.getWorldWidth(), world.getWorldHeight());
    camera.setCreativeMode(creativeMode);
    
    // Font for chunk info
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
    
    // Main game loop
    sf::Clock clock;
    while (window.isOpen()) {
        // Time delta for smooth movement
        float dt = clock.restart().asSeconds();
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                
                // Generate new world on space press
                if (event.key.code == sf::Keyboard::Space) {
                    seed = dis(gen);
                    std::cout << "Generated new world with seed: " << seed << std::endl;
                    world.reset(seed);
                    // Reset camera position
                    camera.reset();
                }
                
                // Toggle creative mode
                if (event.key.code == sf::Keyboard::C) {
                    creativeMode = !creativeMode;
                    camera.setCreativeMode(creativeMode);
                    std::cout << "Creative mode: " << (creativeMode ? "ON" : "OFF") << std::endl;
                }
                
                // Zoom controls
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
                    camera.zoom(0.9f); // Zoom in
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash)
                    camera.zoom(1.1f); // Zoom out
                if (event.key.code == sf::Keyboard::Num0)
                    camera.reset(); // Reset view
            }
        }
        
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
            // Normalize for diagonal movement (prevents faster diagonal speed)
            if (dx != 0.0f && dy != 0.0f) {
                dx *= 0.7071f; // 1/sqrt(2)
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
        
        // Update the view
        window.setView(view);
        
        // Clear the window
        window.clear(sf::Color(135, 206, 235)); // Sky blue background
        
        // Draw the world
        world.draw(window);
        
        // Draw UI elements with fixed position relative to the view
        sf::View prevView = window.getView();
        sf::View uiView = window.getDefaultView();
        window.setView(uiView);
        
        // Position the text in the top-left corner
        chunkText.setPosition(10, 10);
        window.draw(chunkText);
        
        // Restore the game view
        window.setView(prevView);
        
        // Display the window
        window.display();
    }
    
    return 0;
} 