#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const sf::Font& font);
    
    void draw(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void setCallback(const std::function<void()>& callback);
    void setHighlighted(bool highlighted);
    
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> callback;
    bool highlighted = false;
};

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window);
    ~MainMenu();
    
    bool initialize();
    void handleEvent(const sf::Event& event);
    void update();
    void draw();
    bool isActive() const;
    void setActive(bool active);
    
    // Callback setters
    void setNewWorldCallback(const std::function<void()>& callback);
    void setExitCallback(const std::function<void()>& callback);
    
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text titleText;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    
    std::vector<Button> buttons;
    bool active = true;
    
    std::function<void()> newWorldCallback;
    std::function<void()> exitCallback;
}; 