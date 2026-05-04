#pragma message(">>> LazyMouse.h IS INCLUDED <<<")
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"

class ParkourGame
{
public:
    void init(sf::RenderWindow& window);
    void start();
    void reset();
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    // =========================
    // 地形系统
    float getGroundHeight(float x);
    bool isGap(float x);

    // =========================
    // 奶酪生成（3/4/5连串）
    void generateCheeseChain(float baseX);

    // =========================
    // 重置奶酪
    void resetCheese();

public:
    // =========================
    // 资源（皮肤系统）

    sf::Texture mouseTex;          // 默认皮肤
    sf::Texture mousePlusTex;      // ⭐保留（兼容旧版本）
    sf::Texture mousePlusPlusTex;  // ⭐52分进化皮肤

    sf::Texture cheeseTex;

    sf::Sprite mouse;
    std::vector<sf::Sprite> cheeses;

    // =========================
    // 物理系统
    sf::Vector2f velocity;

    float gravity = 0.6f;
    float jumpPower = -12.f;

    bool onGround = false;

    // =========================
    // 游戏状态
    GameState* state = nullptr;

    float cameraX = 0.f;
    float score = 0.f;
    float finalScore = 0.f;

    bool isFalling = false;

    float lastCheeseX = 0.f;

    // =========================
    // UI
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
};