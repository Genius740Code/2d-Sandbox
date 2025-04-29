#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include "Button.h"

enum class GameState {
    MAIN_MENU,
    WORLD_CREATION,
    GAME,
    EXIT
};

enum class GameMode {
    SURVIVAL,
    CREATIVE,
    HARDCORE
};

enum class Difficulty {
    PEACEFUL,
    EASY,
    NORMAL,
    HARD
};

class MenuState {
private:
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Text title;
    
    std::vector<Button> buttons;
    
    // World creation settings
    std::string worldName;
    GameMode gameMode;
    Difficulty difficulty;
    
    // Text fields for world creation
    sf::Text worldNameText;
    sf::Text worldNameLabel;
    sf::Text gameModeLabel;
    sf::Text difficultyLabel;
    
    sf::RectangleShape textInputBox;
    bool isInputActive;
    
    Button modeButton;
    Button difficultyButton;
    
    // Callback for state changes
    std::function<void(GameState)> onStateChange;
    std::function<void(const std::string&, GameMode, Difficulty)> onCreateWorld;

public:
    MenuState();
    
    bool initialize();
    void setupMainMenu();
    void setupWorldCreation();
    
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    
    void setOnStateChange(std::function<void(GameState)> callback);
    void setOnCreateWorld(std::function<void(const std::string&, GameMode, Difficulty)> callback);
    
    void cycleGameMode();
    void cycleDifficulty();
    
    std::string getGameModeString() const;
    std::string getDifficultyString() const;
    
    GameMode getGameMode() const { return gameMode; }
    Difficulty getDifficulty() const { return difficulty; }
    std::string getWorldName() const { return worldName; }
}; 