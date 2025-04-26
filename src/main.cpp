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
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Minecraft Terrain Generation");
    window.setFramerateLimit(60);
    
    // World parameters
    const int worldWidth = 400;  // Wider world
    const int worldHeight = 200; // Taller world
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
    
    // Print the seed
    std::cout << "Generated world with seed: " << seed << std::endl;
    std::cout << "The world now has procedurally generated terrain!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD/Arrows: Move camera" << std::endl;
    std::cout << "  C: Toggle creative mode (affects movement speed)" << std::endl;
    std::cout << "  +/-: Zoom in/out" << std::endl;
    std::cout << "  0: Reset view" << std::endl;
    std::cout << "  Space: Generate new world" << std::endl;
    std::cout << "  Esc: Exit" << std::endl;
    
    // Create the world
    World world(worldWidth, worldHeight, tileSize, seed);
    
    // Create camera
    Camera camera(windowWidth, windowHeight, worldWidth * tileSize, worldHeight * tileSize);
    camera.setCreativeMode(creativeMode);
    
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
        
        // Update the view
        window.setView(camera.getView());
        
        // Clear the window
        window.clear(sf::Color(135, 206, 235)); // Sky blue background
        
        // Draw the world
        world.draw(window);
        
        // Display the window
        window.display();
    }
    
    return 0;
} 