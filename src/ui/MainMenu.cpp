#include "MainMenu.h"
#include <iostream>

// Button implementation
Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& buttonText, const sf::Font& font) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(200, 200, 200));
    
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    
    // Center text in button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - textBounds.top
    );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setCallback(const std::function<void()>& cb) {
    callback = cb;
}

void Button::setHighlighted(bool isHighlighted) {
    highlighted = isHighlighted;
    if (highlighted) {
        shape.setFillColor(sf::Color(150, 150, 150));
    } else {
        shape.setFillColor(sf::Color(100, 100, 100));
    }
}

// MainMenu implementation
MainMenu::MainMenu(sf::RenderWindow& win) : window(win) {
    
}

MainMenu::~MainMenu() {
    
}

bool MainMenu::initialize() {
    // Load font
    if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
        std::cerr << "Failed to load font for main menu!" << std::endl;
        return false;
    }
    
    // Set up title text
    titleText.setFont(font);
    titleText.setString("2D MINECRAFT");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3);
    
    // Center title text at the top of the window
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        (window.getSize().x - titleBounds.width) / 2,
        50 - titleBounds.top
    );
    
    // Create buttons
    float buttonWidth = 300;
    float buttonHeight = 60;
    float buttonSpacing = 20;
    float startY = 200;
    
    // New World button
    buttons.emplace_back(
        sf::Vector2f((window.getSize().x - buttonWidth) / 2, startY),
        sf::Vector2f(buttonWidth, buttonHeight),
        "New World",
        font
    );
    
    // Options button (placeholder for future expansion)
    buttons.emplace_back(
        sf::Vector2f((window.getSize().x - buttonWidth) / 2, startY + buttonHeight + buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Options",
        font
    );
    
    // Exit button
    buttons.emplace_back(
        sf::Vector2f((window.getSize().x - buttonWidth) / 2, startY + 2 * (buttonHeight + buttonSpacing)),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Exit",
        font
    );
    
    return true;
}

void MainMenu::handleEvent(const sf::Event& event) {
    if (!active) return;
    
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        
        // Update button highlights
        for (auto& button : buttons) {
            button.setHighlighted(button.isMouseOver(mousePos));
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        
        // Check if any button was clicked
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].isMouseOver(mousePos)) {
                // First button (New World)
                if (i == 0 && newWorldCallback) {
                    newWorldCallback();
                }
                // Third button (Exit)
                else if (i == 2 && exitCallback) {
                    exitCallback();
                }
            }
        }
    }
}

void MainMenu::update() {
    // Update animations or other state changes if needed
}

void MainMenu::draw() {
    if (!active) return;
    
    // Draw background (currently just a solid color)
    window.clear(sf::Color(50, 50, 150));
    
    // Draw title
    window.draw(titleText);
    
    // Draw buttons
    for (auto& button : buttons) {
        button.draw(window);
    }
}

bool MainMenu::isActive() const {
    return active;
}

void MainMenu::setActive(bool isActive) {
    active = isActive;
}

void MainMenu::setNewWorldCallback(const std::function<void()>& callback) {
    newWorldCallback = callback;
    buttons[0].setCallback(callback);
}

void MainMenu::setExitCallback(const std::function<void()>& callback) {
    exitCallback = callback;
    buttons[2].setCallback(callback);
} 