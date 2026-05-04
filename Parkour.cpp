#include "Parkour.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

// ===============================
void ParkourGame::resetCheese()
{
    cheeses.clear();
}

// ===============================
float ParkourGame::getGroundHeight(float x)
{
    int ix = (int)x;

    if (ix < 0 || ix >= (int)groundMap.size())
        return -1.f;

    return groundMap[ix];
}

// ===============================
bool ParkourGame::isGap(float x)
{
    return getGroundHeight(x) < 0.f;
}

// ===============================
void ParkourGame::init(sf::RenderWindow& window)
{
    mouseTex.loadFromFile("D:/VS project/鼠鼠大作战/images/mouse.png");
    mousePlusPlusTex.loadFromFile("images/mouse++.png");
    cheeseTex.loadFromFile("images/cheese.png");
    font.loadFromFile("D:/VS project/鼠鼠大作战/x64/Debug/simhei.ttf");

    mouse.setTexture(mouseTex);

    float targetW = 80.f;
    float targetH = 80.f;

    auto ts = mouseTex.getSize();
    mouse.setScale(targetW / ts.x, targetH / ts.y);

    groundMap.resize(50000);

    for (int x = 0; x < 50000; x++)
    {
        int seg = x / 200;

        if (seg % 8 == 0)
        {
            groundMap[x] = -1.f;
            continue;
        }

        groundMap[x] =
            520.f +
            std::sin(x * 0.01f) * 60.f +
            std::sin(x * 0.003f) * 80.f;
    }

    start();
}

// ===============================
void ParkourGame::start()
{
    playerWorldX = 200.f;
    playerWorldY = getGroundHeight(playerWorldX);

    velocity = { 0.f, 0.f };

    score = 0.f;
    finalScore = 0.f;

    onGround = false;
    isFalling = false;

    resetCheese();
    lastCheeseX = 0.f;
}

// ===============================
void ParkourGame::reset()
{
    start();
}

// ===============================
// ⭐奶酪系统（仅降低密度版本）
void ParkourGame::generateCheeseChain(float baseX)
{
    // 🔻降低生成数量：3~5 → 2~3
    int pattern[3] = { 2, 3, 3 };
    int count = pattern[rand() % 3];

    // 🔻增加间距：55 → 75
    float spacing = 75.f;
    float offset = -(count / 2.f) * spacing;

    float cheeseH = cheeseTex.getSize().y * 0.06f;

    for (int i = 0; i < count; i++)
    {
        float x = baseX + offset + i * spacing;

        float x1 = x - 10.f;
        float x2 = x + 10.f;

        float g1 = getGroundHeight(x1);
        float g2 = getGroundHeight(x2);

        if (g1 < 0.f || g2 < 0.f)
            continue;

        float g = (g1 + g2) * 0.5f;

        sf::Sprite c;
        c.setTexture(cheeseTex);
        c.setScale(0.06f, 0.06f);
        c.setPosition(x, g - cheeseH - 2.f);

        cheeses.push_back(c);
    }
}

// ===============================
void ParkourGame::update(sf::RenderWindow& window)
{
    if (*state == GameOver)
        return;

    float speed = 6.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        playerWorldX += speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        playerWorldX -= speed;

    float playerH = mouse.getGlobalBounds().height;

    float groundNow = getGroundHeight(playerWorldX);
    bool hasGround = (groundNow >= 0.f);

    // =========================
    velocity.y += gravity * 0.4f;

    bool grounded = false;

    if (hasGround && playerWorldY >= groundNow - playerH && velocity.y >= 0.f)
    {
        playerWorldY = groundNow - playerH;
        velocity.y = 0.f;
        grounded = true;
        onGround = true;
    }
    else
    {
        playerWorldY += velocity.y;
        onGround = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && grounded)
    {
        velocity.y = -18.f;
    }

    float nextGround = getGroundHeight(playerWorldX + 120.f);
    bool atCliff = hasGround && (nextGround < 0.f);

    if (atCliff)
    {
        isFalling = true;
    }

    if (isFalling)
    {
        velocity.y += gravity * 1.6f;
        playerWorldY += velocity.y;
    }

    if (playerWorldY > window.getSize().y + 200.f)
    {
        finalScore = score;
        *state = GameOver;
        return;
    }

    float screenX = window.getSize().x / 2.f;
    cameraX = playerWorldX - screenX;

    // 🔻降低生成频率：300 → 450
    if (playerWorldX - lastCheeseX > 450.f)
    {
        generateCheeseChain(playerWorldX + 700.f);
        lastCheeseX = playerWorldX;
    }

    sf::FloatRect playerRect(
        playerWorldX,
        playerWorldY,
        mouse.getGlobalBounds().width,
        mouse.getGlobalBounds().height
    );

    for (int i = 0; i < cheeses.size(); )
    {
        if (playerRect.intersects(cheeses[i].getGlobalBounds()))
        {
            score += 1.f;
            cheeses.erase(cheeses.begin() + i);
        }
        else
            i++;
    }
}

// ===============================
void ParkourGame::draw(sf::RenderWindow& window)
{
    float screenX = window.getSize().x / 2.f;

    mouse.setPosition(screenX, playerWorldY);
    window.draw(mouse);

    for (int i = 0; i < window.getSize().x; i += 4)
    {
        float x = cameraX + i;
        float y = getGroundHeight(x);

        if (y < 0.f) continue;

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i, y), sf::Color(139,69,19)),
            sf::Vertex(sf::Vector2f(i, window.getSize().y), sf::Color(139,69,19))
        };

        window.draw(line, 2, sf::Lines);
    }

    for (auto& c : cheeses)
    {
        sf::Sprite temp = c;
        temp.move(-cameraX, 0.f);
        window.draw(temp);
    }

    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string((int)score));
    scoreText.setPosition(10.f, 10.f);
    window.draw(scoreText);

    if (*state == GameOver)
    {
        gameOverText.setFont(font);
        gameOverText.setString("Game Over\nFinal: " + std::to_string((int)finalScore));
        gameOverText.setPosition(400.f, 300.f);
        window.draw(gameOverText);
    }
}