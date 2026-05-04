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

public:
    GameState* state = nullptr;

    float finalScore = 0.f;

private:
    // =========================
    // 地形
    float getGroundHeight(float x);
    bool isGap(float x);

    std::vector<float> groundMap;

    // =========================
    // 奶酪
    void generateCheeseChain(float baseX);
    void resetCheese();

    std::vector<sf::Sprite> cheeses;
    float lastCheeseX = 0.f;

    // =========================
    // 玩家（world坐标）
    float playerWorldX = 200.f;
    float playerWorldY = 0.f;

    // =========================
    // 物理
    sf::Vector2f velocity;
    float gravity = 0.6f;

    bool onGround = false;

    // ⭐新增：掉落状态（死亡逻辑用）
    bool isFalling = false;

    // =========================
    // 摄像机
    float cameraX = 0.f;

    // =========================
    // 分数
    float score = 0.f;

    // =========================
    // 资源
    sf::Texture mouseTex;
    sf::Texture mousePlusPlusTex;
    sf::Texture cheeseTex;

    sf::Sprite mouse;

    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
};