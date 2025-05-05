#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

// Camera class to handle movement and boundaries
class Camera {
private:
    sf::View view;
    int viewportWidth, viewportHeight;
    int worldWidth, worldHeight;
    float moveSpeed;
    
    // Helper method to keep coordinates within world boundaries
    float applyBoundary(float value, float viewportDimension, float worldDimension) const;

public:
    Camera(int vpWidth, int vpHeight, int wWidth, int wHeight);
    void move(float dx, float dy, float dt);
    void setSpeed(float speed);
    void zoom(float factor);
    const sf::View& getView() const;
    void setCreativeMode(bool isCreative);
    void reset();
    void setPosition(float x, float y);
}; 