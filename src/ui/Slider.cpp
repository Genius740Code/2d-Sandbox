#include "MenuState.h"

Slider::Slider(sf::Font* font, float min, float max, float initial, 
               sf::Vector2f size, sf::Vector2f position) 
    : font(font), minValue(min), maxValue(max), currentValue(initial), isDragging(false) {
    
    // Configure the track
    track.setSize(size);
    track.setPosition(position);
    track.setFillColor(sf::Color(80, 80, 80, 200));
    track.setOutlineThickness(1);
    track.setOutlineColor(sf::Color(40, 40, 40, 200));
    
    // Configure the handle
    float handleWidth = 20.0f;
    float handleHeight = size.y + 10.0f;
    handle.setSize(sf::Vector2f(handleWidth, handleHeight));
    
    // Set initial handle position based on value
    float percent = (initial - min) / (max - min);
    float handleX = position.x + percent * (size.x - handleWidth);
    handle.setPosition(sf::Vector2f(handleX, position.y - 5.0f));
    handle.setFillColor(sf::Color(180, 180, 180, 230));
    handle.setOutlineThickness(1);
    handle.setOutlineColor(sf::Color(40, 40, 40, 200));
    
    // Configure the value text display
    if (font) {
        valueText.setFont(*font);
        valueText.setCharacterSize(18);
        valueText.setFillColor(sf::Color::White);
        valueText.setPosition(position.x + size.x + 20.0f, position.y);
        updateText();
    }
}

void Slider::setPosition(sf::Vector2f position) {
    track.setPosition(position);
    
    // Calculate handle's x position based on current value
    float percent = (currentValue - minValue) / (maxValue - minValue);
    float handleX = position.x + percent * (track.getSize().x - handle.getSize().x);
    handle.setPosition(sf::Vector2f(handleX, position.y - 5.0f));
    
    valueText.setPosition(position.x + track.getSize().x + 20.0f, position.y);
}

void Slider::setValue(float value) {
    // Clamp value to range
    currentValue = std::max(minValue, std::min(maxValue, value));
    
    // Update handle position
    float percent = (currentValue - minValue) / (maxValue - minValue);
    float handleX = track.getPosition().x + percent * (track.getSize().x - handle.getSize().x);
    handle.setPosition(sf::Vector2f(handleX, handle.getPosition().y));
    
    updateText();
    
    // Call the value change callback if set
    if (onValueChange) {
        onValueChange(currentValue);
    }
}

float Slider::getValue() const {
    return currentValue;
}

void Slider::setOnValueChange(std::function<void(float)> callback) {
    onValueChange = callback;
}

void Slider::updateText() {
    if (currentValue == 0) {
        valueText.setString("Unlimited FPS");
    } else {
        valueText.setString(std::to_string(static_cast<int>(currentValue)) + " FPS");
    }
}

bool Slider::contains(sf::Vector2f point) const {
    return track.getGlobalBounds().contains(point);
}

bool Slider::handleContains(sf::Vector2f point) const {
    return handle.getGlobalBounds().contains(point);
}

void Slider::update(sf::Vector2f mousePos) {
    if (isDragging) {
        // Calculate value based on handle position
        float trackStart = track.getPosition().x;
        float trackEnd = trackStart + track.getSize().x - handle.getSize().x;
        float handleX = std::max(trackStart, std::min(trackEnd, mousePos.x - handle.getSize().x / 2));
        
        // Update handle position
        handle.setPosition(sf::Vector2f(handleX, handle.getPosition().y));
        
        // Calculate and set the new value
        float percent = (handleX - trackStart) / (trackEnd - trackStart);
        float newValue = minValue + percent * (maxValue - minValue);
        
        // Snap to preset values: 30, 60, 120, 144, 240, 0 (unlimited)
        float snapValues[] = {30, 60, 120, 144, 240, 0};
        float closestValue = snapValues[0];
        float minDiff = std::abs(newValue - snapValues[0]);
        
        for (int i = 1; i < 6; i++) {
            float diff = std::abs(newValue - snapValues[i]);
            if (diff < minDiff) {
                minDiff = diff;
                closestValue = snapValues[i];
            }
        }
        
        if (minDiff < (maxValue - minValue) / 15.0f) { // Snap threshold
            newValue = closestValue;
        }
        
        // Only update if value changed
        if (newValue != currentValue) {
            setValue(newValue);
        }
    }
}

void Slider::handleEvent(const sf::Event& event, sf::Vector2f mousePos) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (handleContains(mousePos)) {
                isDragging = true;
            } else if (contains(mousePos)) {
                // Jump to clicked position
                float trackStart = track.getPosition().x;
                float trackEnd = trackStart + track.getSize().x - handle.getSize().x;
                float handleX = std::max(trackStart, std::min(trackEnd, mousePos.x - handle.getSize().x / 2));
                
                handle.setPosition(sf::Vector2f(handleX, handle.getPosition().y));
                
                // Calculate and set the new value
                float percent = (handleX - trackStart) / (trackEnd - trackStart);
                float newValue = minValue + percent * (maxValue - minValue);
                setValue(newValue);
                
                isDragging = true;
            }
        }
    } 
    else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
        }
    }
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(handle);
    window.draw(valueText);
} 