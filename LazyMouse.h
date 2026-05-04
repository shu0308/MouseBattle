#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class LazyMouse
{
public:
    void init(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    // ⭐新增：用于判断MENU按钮点击
    bool isBackClicked(sf::Vector2f mousePos) const;

    GameState* state = nullptr;

private:
    sf::Texture phoneTex, phonePlusTex, phonePlusPlusTex, sleepTex;
    sf::Texture cheeseTex, badCheeseTex;

    sf::Sprite sprite;
    sf::Sprite cheeseButton;
    sf::Sprite badCheeseButton;

    sf::RectangleShape backBtn;
    sf::Text backText;

    sf::Font font;
    sf::Text clockText;
    sf::Text moodText;

    int mood = 0;
    int bgState = 0;

    bool mouseDownLast = false;
    bool lockBg = false;
};