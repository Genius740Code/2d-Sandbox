#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font* font;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    sf::Color textColor;
    
    bool isHovered;
    bool isActive;
    
    std::function<void()> onClick;

public:
    Button(sf::Font* font, const std::string& text, sf::Vector2f size, sf::Vector2f position);
    
    void setColors(sf::Color idle, sf::Color hover, sf::Color active, sf::Color text);
    void setPosition(sf::Vector2f position);
    void setOnClick(std::function<void()> callback);
    void setText(const std::string& newText);
    
    bool contains(sf::Vector2f point) const;
    void update(sf::Vector2f mousePos);
    void handleEvent(const sf::Event& event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    std::string getText() const;
}; 