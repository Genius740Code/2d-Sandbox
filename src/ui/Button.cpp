#include "Button.h"

Button::Button(sf::Font* font, const std::string& buttonText, sf::Vector2f size, sf::Vector2f position) 
    : font(font), isHovered(false), isActive(false) {
    
    // Configure the button shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(20, 20, 20, 200));
    
    // Default colors - will likely be overridden by setColors
    idleColor = sf::Color(100, 100, 100, 200);
    hoverColor = sf::Color(150, 150, 150, 200);
    activeColor = sf::Color(70, 70, 70, 200);
    textColor = sf::Color::White;
    
    shape.setFillColor(idleColor);
    
    // Configure the text
    text.setFont(*font);
    text.setString(buttonText);
    text.setFillColor(textColor);
    text.setCharacterSize(24);
    
    // Center the text on the button
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::setColors(sf::Color idle, sf::Color hover, sf::Color active, sf::Color text) {
    idleColor = idle;
    hoverColor = hover;
    activeColor = active;
    textColor = text;
    
    this->text.setFillColor(textColor);
    shape.setFillColor(idleColor);
}

void Button::setPosition(sf::Vector2f position) {
    shape.setPosition(position);
    text.setPosition(
        position.x + shape.getSize().x / 2, 
        position.y + shape.getSize().y / 2 - text.getCharacterSize() / 3
    );
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
    
    // Re-center the text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    
    // Maintain position at the center of the button
    sf::Vector2f buttonPos = shape.getPosition();
    sf::Vector2f buttonSize = shape.getSize();
    text.setPosition(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f);
}

bool Button::contains(sf::Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

void Button::update(sf::Vector2f mousePos) {
    isHovered = contains(mousePos);
    
    if (isActive) {
        shape.setFillColor(activeColor);
    } else if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }
}

void Button::handleEvent(const sf::Event& event, sf::Vector2f mousePos) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && contains(mousePos)) {
            isActive = true;
            shape.setFillColor(activeColor);
        }
    } 
    else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (isActive && contains(mousePos) && onClick) {
                onClick();
            }
            isActive = false;
        }
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

sf::Vector2f Button::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Button::getSize() const {
    return shape.getSize();
}

std::string Button::getText() const {
    return text.getString();
} 