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

// Helper function to keep coordinates within world boundaries
float Camera::applyBoundary(float value, float viewportDimension, float worldDimension) const {
    return std::max(viewportDimension / 4.0f, std::min(value, worldDimension - viewportDimension / 4.0f));
}

void Camera::move(float dx, float dy, float dt) {
    // Calculate new position with delta time
    float newX = view.getCenter().x + dx * moveSpeed * dt;
    float newY = view.getCenter().y + dy * moveSpeed * dt;
    
    // Apply bounds checking using the helper function
    newX = applyBoundary(newX, viewportWidth, worldWidth);
    newY = applyBoundary(newY, viewportHeight, worldHeight);
    
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

void Camera::setPosition(float x, float y) {
    // Apply bounds checking to keep view within world boundaries using the helper function
    float boundedX = applyBoundary(x, viewportWidth, worldWidth);
    float boundedY = applyBoundary(y, viewportHeight, worldHeight);
    
    // Set the new center position directly
    view.setCenter(boundedX, boundedY);
} 