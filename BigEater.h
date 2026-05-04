#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "GameState.h"

struct Game
{
    GameState state = MainMenu;

    sf::Font font;

    sf::Texture bgTex;
    sf::Sprite background;

    sf::Music bgMusic;
    sf::Music modeMusic;

    sf::Texture mouseTex, mousePlusTex, catTex, cheeseTex, badCheeseTex;

    sf::Sprite mouse, cat;

    std::vector<sf::Sprite> cheeses;
    std::vector<sf::Vector2f> cheeseVel;
    std::vector<sf::Sprite> badCheeses;

    sf::Vector2f mousePos;
    sf::Vector2f catPos;

    sf::Vector2f offset = { 300, 200 };

    int score = 0;
    int finalScore = 0;

    bool upgraded = false;
    float speedMultiplier = 1.0f;

    sf::Vector2u screen;

    const int MAX_CHEESE = 30;

    void spawnCheese();
    void spawnBadCheese();

    void init(sf::RenderWindow& window);
    void startBigEater();
    void resetToMenu();

    void updateBigEater();
};