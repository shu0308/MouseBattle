#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"

class LazyMouse
{
public:
    void init(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    bool isBackClicked(sf::Vector2f mousePos) const;

public:
    GameState* state = nullptr;

private:
    sf::Texture phoneTex;
    sf::Texture phonePlusTex;
    sf::Texture phonePlusPlusTex;
    sf::Texture sleepTex;

    sf::Texture cheeseTex;
    sf::Texture badCheeseTex;

    sf::Sprite sprite;

    sf::Sprite cheeseButton;
    sf::Sprite badCheeseButton;

    sf::RectangleShape backBtn;
    sf::Text backText;

    sf::Text clockText;
    sf::Text moodText;
    sf::Font font;

    int mood = 50;
    int bgState = 0;

    bool mouseDownLast = false;
    bool lockBg = false;

    float lockTimer = 0.f;

    // ⭐亮度遮罩
    sf::RectangleShape brightnessOverlay;
};