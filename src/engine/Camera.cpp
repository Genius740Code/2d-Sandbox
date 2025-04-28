#include "Camera.h"

Camera::Camera(int vpWidth, int vpHeight, int wWidth, int wHeight) : 
    viewportWidth(vpWidth),
    viewportHeight(vpHeight),
    worldWidth(wWidth),
    worldHeight(wHeight),
    moveSpeed(500.0f) {
    
    // Initialize view with a closer zoom (smaller size = closer zoom)
    view.setSize(static_cast<float>(vpWidth) * 0.7f, static_cast<float>(vpHeight) * 0.7f);
    view.setCenter(wWidth / 2.0f, wHeight / 3.0f); // Position to see more of the surface
}

void Camera::move(float dx, float dy, float dt) {
    // Calculate new position with delta time
    float newX = view.getCenter().x + dx * moveSpeed * dt;
    float newY = view.getCenter().y + dy * moveSpeed * dt;
    
    // Apply bounds checking
    newX = std::max(viewportWidth / 4.0f, std::min(newX, worldWidth - viewportWidth / 4.0f));
    newY = std::max(viewportHeight / 4.0f, std::min(newY, worldHeight - viewportHeight / 4.0f));
    
    // Set the new center
    view.setCenter(newX, newY);
}

void Camera::setSpeed(float speed) {
    moveSpeed = speed;
}

void Camera::zoom(float factor) {
    // Apply zoom
    view.zoom(factor);
}

const sf::View& Camera::getView() const {
    return view;
}

void Camera::setCreativeMode(bool isCreative) {
    // In creative mode, movement is faster
    moveSpeed = isCreative ? 1000.0f : 500.0f;
}

void Camera::reset() {
    // Center horizontally but position vertically to see more of the surface
    view.setCenter(worldWidth / 2.0f, worldHeight / 3.0f);
    // Reset zoom to closer default view
    view.setSize(static_cast<float>(viewportWidth) * 0.7f, static_cast<float>(viewportHeight) * 0.7f);
} 