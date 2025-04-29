#include "MenuState.h"
#include <iostream>

MenuState::MenuState() : 
    worldName("New World"),
    gameMode(GameMode::SURVIVAL), 
    difficulty(Difficulty::NORMAL),
    isInputActive(false),
    modeButton(&font, "SURVIVAL", sf::Vector2f(200, 50), sf::Vector2f(0, 0)),
    difficultyButton(&font, "NORMAL", sf::Vector2f(200, 50), sf::Vector2f(0, 0))
{
    // Set defaults for callbacks
    onStateChange = [](GameState) {};
    onCreateWorld = [](const std::string&, GameMode, Difficulty) {};
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
    // Currently no functionality, could be expanded later
    
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
    
    // Set the title
    title.setString("Create New World");
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(640, 100);
    
    // Define button size and colors for a more modern and appealing look
    sf::Vector2f buttonSize(220, 50);
    sf::Color buttonIdle(56, 56, 56, 230);
    sf::Color buttonHover(90, 90, 90, 230);
    sf::Color buttonActive(35, 35, 35, 230);
    sf::Color textColor(255, 255, 255);
    sf::Color accentColor(32, 168, 96);
    
    // World name input field
    worldNameLabel.setFont(font);
    worldNameLabel.setCharacterSize(24);
    worldNameLabel.setString("World Name:");
    worldNameLabel.setFillColor(sf::Color::White);
    worldNameLabel.setOutlineColor(sf::Color::Black);
    worldNameLabel.setOutlineThickness(1);
    worldNameLabel.setPosition(440, 200);
    
    textInputBox.setSize(sf::Vector2f(300, 40));
    textInputBox.setPosition(640, 200);
    textInputBox.setFillColor(sf::Color(25, 25, 25, 230));
    textInputBox.setOutlineColor(sf::Color::White);
    textInputBox.setOutlineThickness(2);
    
    worldNameText.setFont(font);
    worldNameText.setCharacterSize(20);
    worldNameText.setString(worldName);
    worldNameText.setFillColor(sf::Color::White);
    worldNameText.setPosition(650, 210);
    
    // Game mode selector
    gameModeLabel.setFont(font);
    gameModeLabel.setCharacterSize(24);
    gameModeLabel.setString("Game Mode:");
    gameModeLabel.setFillColor(sf::Color::White);
    gameModeLabel.setOutlineColor(sf::Color::Black);
    gameModeLabel.setOutlineThickness(1);
    gameModeLabel.setPosition(440, 280);
    
    modeButton = Button(&font, getGameModeString(), buttonSize, sf::Vector2f(640, 280));
    modeButton.setColors(buttonIdle, buttonHover, buttonActive, textColor);
    modeButton.setOnClick([this]() {
        cycleGameMode();
        modeButton.setText(getGameModeString());
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
        difficultyButton.setText(getDifficultyString());
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

void MenuState::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // Handle button events
    for (auto& button : buttons) {
        button.handleEvent(event, mousePos);
    }
    
    // Text input handling (for world creation menu)
    if (buttons.size() > 3) { // This is a hacky way to check if we're in world creation menu
        if (event.type == sf::Event::MouseButtonPressed) {
            bool wasInputActive = isInputActive;
            isInputActive = textInputBox.getGlobalBounds().contains(mousePos);
            
            if (isInputActive) {
                textInputBox.setOutlineColor(sf::Color::Yellow);
                
                // Clear default text on first click
                if (!wasInputActive && worldName == "New World") {
                    worldName = "";
                    worldNameText.setString(worldName);
                }
            } else {
                textInputBox.setOutlineColor(sf::Color::White);
                
                // If text is empty when clicking away, restore default
                if (worldName.empty()) {
                    worldName = "New World";
                    worldNameText.setString(worldName);
                }
            }
        }
        
        if (isInputActive && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!worldName.empty()) {
                    worldName.pop_back();
                }
            }
            else if (event.text.unicode == 13) { // Enter
                isInputActive = false;
                textInputBox.setOutlineColor(sf::Color::White);
                
                // If text is empty when pressing enter, restore default
                if (worldName.empty()) {
                    worldName = "New World";
                }
            }
            else if (event.text.unicode < 128 && event.text.unicode != 8) {
                if (worldName.size() < 20) { // Limit the world name length
                    worldName += static_cast<char>(event.text.unicode);
                }
            }
            
            worldNameText.setString(worldName);
        }
    }
}

void MenuState::update(sf::Vector2f mousePos) {
    for (auto& button : buttons) {
        button.update(mousePos);
    }
}

void MenuState::draw(sf::RenderWindow& window) {
    // Draw background with a gradient effect if texture isn't available
    if (backgroundTexture.getSize().x > 0) {
        // Scale background to fill window
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = backgroundTexture.getSize();
        
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        
        background.setScale(scaleX, scaleY);
        window.draw(background);
    } else {
        // Create a gradient effect as fallback
        sf::RectangleShape darkOverlay;
        darkOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        darkOverlay.setPosition(0, 0);
        
        // Create gradient background with Minecraft-like colors
        sf::RectangleShape gradientTop;
        gradientTop.setSize(sf::Vector2f(window.getSize().x, window.getSize().y/2));
        gradientTop.setPosition(0, 0);
        gradientTop.setFillColor(sf::Color(70, 120, 200)); // Sky blue
        
        sf::RectangleShape gradientBottom;
        gradientBottom.setSize(sf::Vector2f(window.getSize().x, window.getSize().y/2));
        gradientBottom.setPosition(0, window.getSize().y/2);
        gradientBottom.setFillColor(sf::Color(50, 90, 160)); // Darker blue
        
        // Create semi-transparent overlay for text contrast
        darkOverlay.setFillColor(sf::Color(0, 0, 0, 120));
        
        window.draw(gradientTop);
        window.draw(gradientBottom);
        window.draw(darkOverlay);
    }
    
    // Draw title with a slight shadow effect
    sf::Text titleShadow = title;
    titleShadow.setPosition(title.getPosition() + sf::Vector2f(3, 3));
    titleShadow.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(titleShadow);
    
    // Draw title
    window.draw(title);
    
    // Draw buttons
    for (auto& button : buttons) {
        button.draw(window);
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
    onStateChange = callback;
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