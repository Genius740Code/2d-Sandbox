#include "MenuState.h"
#include <iostream>

MenuState::MenuState() : 
    worldName("New World"),
    gameMode(GameMode::SURVIVAL), 
    difficulty(Difficulty::NORMAL),
    maxFps(60),
    currentState(GameState::MAIN_MENU),
    isInputActive(false),
    modeButton(&font, "SURVIVAL", sf::Vector2f(200, 50), sf::Vector2f(0, 0)),
    difficultyButton(&font, "NORMAL", sf::Vector2f(200, 50), sf::Vector2f(0, 0)),
    fpsButton(&font, "60 FPS", sf::Vector2f(200, 50), sf::Vector2f(0, 0)),
    fpsSlider(&font, 0, 240, 60, sf::Vector2f(200, 20), sf::Vector2f(0, 0))
{
    // Set defaults for callbacks
    onStateChange = [](GameState) {};
    onCreateWorld = [](const std::string&, GameMode, Difficulty) {};
    onMaxFpsChange = [](int) {};
}

bool MenuState::initialize() {
    // Load font
    if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return false;
    }
    
    // Load background texture
    if (!backgroundTexture.loadFromFile("assets/textures/menu_background.png")) {
        // If specific menu background doesn't exist, try to use another texture
        if (!backgroundTexture.loadFromFile("bin/assets/textures/brick_grey.png")) {
            std::cerr << "Failed to load background texture!" << std::endl;
            // Continue anyway, we'll use a solid color as fallback
        }
    }
    
    // Setup background
    background.setTexture(backgroundTexture, true);
    
    // Scale the background to cover the entire window
    background.setScale(
        1280.0f / backgroundTexture.getSize().x,
        720.0f / backgroundTexture.getSize().y
    );
    
    // Set up title with Minecraft-like styling
    title.setFont(font);
    title.setCharacterSize(72);
    title.setFillColor(sf::Color(255, 255, 100)); // Golden yellow
    title.setOutlineColor(sf::Color(100, 60, 0));  // Brown outline
    title.setOutlineThickness(3);
    title.setLetterSpacing(1.2f);  // Slightly spaced out letters
    title.setStyle(sf::Text::Bold);
    
    // Initialize buttons for main menu
    setupMainMenu();
    
    return true;
}

void MenuState::setupMainMenu() {
    buttons.clear();
    currentState = GameState::MAIN_MENU;
    
    // Set the title
    title.setString("2D Minecraft");
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(640, 150);
    
    // Define button size and colors for a Minecraft-like appearance
    sf::Vector2f buttonSize(300, 60);
    sf::Color buttonIdle(56, 56, 56, 230);
    sf::Color buttonHover(90, 90, 90, 230);
    sf::Color buttonActive(35, 35, 35, 230);
    sf::Color textColor(255, 255, 255);
    
    // Create the buttons with more spacing between them
    Button singlePlayerButton(&font, "Singleplayer", buttonSize, sf::Vector2f(640 - buttonSize.x / 2, 280));
    singlePlayerButton.setColors(buttonIdle, buttonHover, buttonActive, textColor);
    singlePlayerButton.setOnClick([this]() {
        setupWorldCreation();
    });
    
    // Add options button
    Button optionsButton(&font, "Options", buttonSize, sf::Vector2f(640 - buttonSize.x / 2, 360));
    optionsButton.setColors(buttonIdle, buttonHover, buttonActive, textColor);
    optionsButton.setOnClick([this]() {
        setupOptions();
        onStateChange(GameState::OPTIONS);
    });
    
    Button exitButton(&font, "Exit Game", buttonSize, sf::Vector2f(640 - buttonSize.x / 2, 440));
    exitButton.setColors(
        sf::Color(180, 60, 60, 230),
        sf::Color(220, 80, 80, 230),
        sf::Color(150, 40, 40, 230),
        textColor
    );
    exitButton.setOnClick([this]() {
        onStateChange(GameState::EXIT);
    });
    
    buttons.push_back(singlePlayerButton);
    buttons.push_back(optionsButton);
    buttons.push_back(exitButton);
}

void MenuState::setupWorldCreation() {
    buttons.clear();
    currentState = GameState::WORLD_CREATION;
    
    title.setString("Create New World");
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(640, 100);
    
    sf::Vector2f buttonSize(200, 50);
    sf::Color buttonIdle(56, 56, 56, 230);
    sf::Color buttonHover(90, 90, 90, 230);
    sf::Color buttonActive(35, 35, 35, 230);
    sf::Color textColor(255, 255, 255);
    
    // World name input
    worldNameLabel.setFont(font);
    worldNameLabel.setCharacterSize(24);
    worldNameLabel.setString("World Name:");
    worldNameLabel.setFillColor(sf::Color::White);
    worldNameLabel.setOutlineColor(sf::Color::Black);
    worldNameLabel.setOutlineThickness(1);
    worldNameLabel.setPosition(440, 220);
    
    // Text input box
    textInputBox.setSize(sf::Vector2f(300, 40));
    textInputBox.setPosition(sf::Vector2f(640, 220));
    textInputBox.setFillColor(sf::Color(30, 30, 30, 200));
    textInputBox.setOutlineThickness(2);
    textInputBox.setOutlineColor(sf::Color(100, 100, 100, 200));
    
    // Text for world name
    worldNameText.setFont(font);
    worldNameText.setCharacterSize(22);
    worldNameText.setString(worldName);
    worldNameText.setFillColor(sf::Color::White);
    worldNameText.setPosition(
        textInputBox.getPosition().x + 10, 
        textInputBox.getPosition().y + 5
    );
    
    // Game mode selector
    gameModeLabel.setFont(font);
    gameModeLabel.setCharacterSize(24);
    gameModeLabel.setString("Game Mode:");
    gameModeLabel.setFillColor(sf::Color::White);
    gameModeLabel.setOutlineColor(sf::Color::Black);
    gameModeLabel.setOutlineThickness(1);
    gameModeLabel.setPosition(440, 290);
    
    modeButton = Button(&font, getGameModeString(), buttonSize, sf::Vector2f(640, 290));
    modeButton.setColors(buttonIdle, buttonHover, buttonActive, textColor);
    
    modeButton.setOnClick([this]() {
        cycleGameMode();
    });
    
    // Difficulty selector
    difficultyLabel.setFont(font);
    difficultyLabel.setCharacterSize(24);
    difficultyLabel.setString("Difficulty:");
    difficultyLabel.setFillColor(sf::Color::White);
    difficultyLabel.setOutlineColor(sf::Color::Black);
    difficultyLabel.setOutlineThickness(1);
    difficultyLabel.setPosition(440, 360);
    
    difficultyButton = Button(&font, getDifficultyString(), buttonSize, sf::Vector2f(640, 360));
    difficultyButton.setColors(buttonIdle, buttonHover, buttonActive, textColor);
    difficultyButton.setOnClick([this]() {
        cycleDifficulty();
    });
    
    // Create World button
    Button createButton(&font, "Create World", sf::Vector2f(280, 60), sf::Vector2f(640, 450));
    createButton.setColors(
        sf::Color(32, 128, 96, 230),
        sf::Color(56, 168, 116, 230), 
        sf::Color(20, 98, 76, 230), 
        textColor
    );
    createButton.setOnClick([this]() {
        onCreateWorld(worldName, gameMode, difficulty);
        onStateChange(GameState::GAME);
    });
    
    // Back button
    Button backButton(&font, "Back", sf::Vector2f(120, 40), sf::Vector2f(100, 650));
    backButton.setColors(
        sf::Color(180, 60, 60, 230),
        sf::Color(220, 80, 80, 230),
        sf::Color(150, 40, 40, 230),
        textColor
    );
    backButton.setOnClick([this]() {
        setupMainMenu();
    });
    
    buttons.push_back(modeButton);
    buttons.push_back(difficultyButton);
    buttons.push_back(createButton);
    buttons.push_back(backButton);
}

void MenuState::setupOptions() {
    buttons.clear();
    currentState = GameState::OPTIONS;
    
    title.setString("Options");
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(640, 100);
    
    sf::Vector2f buttonSize(220, 50);
    sf::Color buttonIdle(56, 56, 56, 230);
    sf::Color buttonHover(90, 90, 90, 230);
    sf::Color buttonActive(35, 35, 35, 230);
    sf::Color textColor(255, 255, 255);
    
    sf::Text fpsLabel;
    fpsLabel.setFont(font);
    fpsLabel.setCharacterSize(24);
    fpsLabel.setString("Max FPS:");
    fpsLabel.setFillColor(sf::Color::White);
    fpsLabel.setOutlineColor(sf::Color::Black);
    fpsLabel.setOutlineThickness(1);
    fpsLabel.setPosition(440, 280);
    
    // Remove fpsButton and replace with slider
    fpsSlider = Slider(&font, 0, 240, maxFps, sf::Vector2f(200, 20), sf::Vector2f(640, 290));
    fpsSlider.setOnValueChange([this](float value) {
        maxFps = static_cast<int>(value);
        onMaxFpsChange(maxFps);
    });
    
    Button backButton(&font, "Back", sf::Vector2f(120, 40), sf::Vector2f(100, 650));
    backButton.setColors(
        sf::Color(180, 60, 60, 230),
        sf::Color(220, 80, 80, 230),
        sf::Color(150, 40, 40, 230),
        textColor
    );
    backButton.setOnClick([this]() {
        setupMainMenu();
        onStateChange(GameState::MAIN_MENU);
    });
    
    // We don't add the slider to the buttons vector
    buttons.push_back(backButton);
}

void MenuState::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Get mouse position in window coordinates
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // Handle button interactions
    for (auto& button : buttons) {
        button.handleEvent(event, mousePos);
    }
    
    // Handle slider interactions if in options menu
    if (currentState == GameState::OPTIONS) {
        fpsSlider.handleEvent(event, mousePos);
    }
    
    // Handle text input for world name
    if (buttons.size() > 3) { // Only in world creation state
        if (event.type == sf::Event::MouseButtonPressed) {
            if (textInputBox.getGlobalBounds().contains(mousePos)) {
                isInputActive = true;
                textInputBox.setOutlineColor(sf::Color(100, 200, 100, 200));
            } else {
                isInputActive = false;
                textInputBox.setOutlineColor(sf::Color(100, 100, 100, 200));
            }
        }
        
        if (isInputActive && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!worldName.empty()) {
                    worldName.pop_back();
                }
            } 
            else if (event.text.unicode == 13) { // Enter key
                isInputActive = false;
            }
            else if (event.text.unicode < 128) { // Regular character
                // Limit world name length to prevent overflow
                if (worldName.length() < 20) {
                    worldName += static_cast<char>(event.text.unicode);
                }
            }
            worldNameText.setString(worldName);
        }
    }
}

void MenuState::update(sf::Vector2f mousePos) {
    // Update all buttons
    for (auto& button : buttons) {
        button.update(mousePos);
    }
    
    // Update the slider if in options menu
    if (currentState == GameState::OPTIONS) {
        fpsSlider.update(mousePos);
    }
}

void MenuState::draw(sf::RenderWindow& window) {
    // Draw the background
    window.draw(background);
    
    // Draw the title
    window.draw(title);
    
    // Draw all buttons
    for (auto& button : buttons) {
        button.draw(window);
    }
    
    // Draw the slider if in options menu
    if (currentState == GameState::OPTIONS) {
        fpsSlider.draw(window);
        // Draw FPS label
        sf::Text fpsLabel;
        fpsLabel.setFont(font);
        fpsLabel.setCharacterSize(24);
        fpsLabel.setString("Max FPS:");
        fpsLabel.setFillColor(sf::Color::White);
        fpsLabel.setOutlineColor(sf::Color::Black);
        fpsLabel.setOutlineThickness(1);
        fpsLabel.setPosition(440, 280);
        window.draw(fpsLabel);
    }
    
    // Draw world creation fields
    if (buttons.size() > 3) { // If we're in world creation menu
        window.draw(worldNameLabel);
        window.draw(textInputBox);
        window.draw(worldNameText);
        window.draw(gameModeLabel);
        window.draw(difficultyLabel);
    }
}

void MenuState::setOnStateChange(std::function<void(GameState)> callback) {
    onStateChange = [this, callback](GameState newState) {
        currentState = newState;
        callback(newState);
    };
}

void MenuState::setOnCreateWorld(std::function<void(const std::string&, GameMode, Difficulty)> callback) {
    onCreateWorld = callback;
}

void MenuState::cycleGameMode() {
    switch (gameMode) {
        case GameMode::SURVIVAL:
            gameMode = GameMode::CREATIVE;
            break;
        case GameMode::CREATIVE:
            gameMode = GameMode::SURVIVAL;
            break;
        // Keep HARDCORE as a special case, but transition to SURVIVAL for simplicity
        case GameMode::HARDCORE:
            gameMode = GameMode::SURVIVAL;
            break;
    }
    
    // Update the button text immediately after changing the game mode
    modeButton.setText(getGameModeString());
}

void MenuState::cycleDifficulty() {
    switch (difficulty) {
        case Difficulty::PEACEFUL:
            difficulty = Difficulty::EASY;
            break;
        case Difficulty::EASY:
            difficulty = Difficulty::NORMAL;
            break;
        case Difficulty::NORMAL:
            difficulty = Difficulty::HARD;
            break;
        case Difficulty::HARD:
            difficulty = Difficulty::PEACEFUL;
            break;
    }
    
    // Update the button text immediately after changing the difficulty
    difficultyButton.setText(getDifficultyString());
}

void MenuState::cycleFps() {
    switch(maxFps) {
        case 30: maxFps = 60; break;
        case 60: maxFps = 120; break;
        case 120: maxFps = 144; break;
        case 144: maxFps = 240; break;
        case 240: maxFps = 0; break;  // 0 means unlimited
        default: maxFps = 30; break;
    }
}

std::string MenuState::getGameModeString() const {
    switch (gameMode) {
        case GameMode::SURVIVAL: return "SURVIVAL";
        case GameMode::CREATIVE: return "CREATIVE";
        case GameMode::HARDCORE: return "HARDCORE";
        default: return "UNKNOWN";
    }
}

std::string MenuState::getDifficultyString() const {
    switch (difficulty) {
        case Difficulty::PEACEFUL: return "PEACEFUL";
        case Difficulty::EASY: return "EASY";
        case Difficulty::NORMAL: return "NORMAL";
        case Difficulty::HARD: return "HARD";
        default: return "UNKNOWN";
    }
}

std::string MenuState::getFpsString() const {
    if (maxFps == 0) {
        return "Unlimited FPS";
    }
    return std::to_string(maxFps) + " FPS";
}

void MenuState::setOnMaxFpsChange(std::function<void(int)> callback) {
    onMaxFpsChange = callback;
} 