#include "BigEater.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

void Game::init(sf::RenderWindow& window)
{
    screen = window.getSize();

    font.loadFromFile("fonts/simhei.ttf");

    bgTex.loadFromFile("images/background.png");
    background.setTexture(bgTex);

    bgMusic.openFromFile("audio/n-buna - 邈豪.ogg");
    bgMusic.setLoop(true);
    bgMusic.setVolume(50.f);
    bgMusic.play();

    modeMusic.openFromFile("audio/n-buna 場秞立弁 - 手丹元五狦互皺歹月井日(2).ogg");
    modeMusic.setLoop(true);
    modeMusic.setVolume(60.f);

    mouseTex.loadFromFile("images/mouse.png");
    mousePlusTex.loadFromFile("images/mouse+.png");
    catTex.loadFromFile("images/cat.png");
    cheeseTex.loadFromFile("images/cheese.png");
    badCheeseTex.loadFromFile("images/badcheese.png");

    mouse.setTexture(mouseTex);
    cat.setTexture(catTex);

    mouse.setScale(0.05f, 0.05f);
    cat.setScale(0.25f, 0.25f);

    mousePos = { screen.x / 2.f - 150, screen.y / 2.f };
    catPos = mousePos + offset;

    mouse.setPosition(mousePos);
    cat.setPosition(catPos);

    cheeses.clear();
    cheeseVel.clear();
    badCheeses.clear();

    for (int i = 0; i < MAX_CHEESE; i++)
        spawnCheese();
}

void Game::startBigEater()
{
    score = 0;
    upgraded = false;
    speedMultiplier = 1.0f;

    mouse.setTexture(mouseTex, true);
    mouse.setScale(0.05f, 0.05f);

    mousePos = { screen.x / 2.f - 150, screen.y / 2.f };
    catPos = mousePos + offset;

    mouse.setPosition(mousePos);
    cat.setPosition(catPos);

    modeMusic.play();
    bgMusic.stop();

    badCheeses.clear();
    for (int i = 0; i < 3; i++)
        spawnBadCheese();
}

void Game::resetToMenu()
{
    score = 0;
    modeMusic.stop();
    bgMusic.play();

    mousePos = { screen.x / 2.f - 150, screen.y / 2.f };
    catPos = mousePos + offset;
}

void Game::spawnCheese()
{
    sf::Sprite c(cheeseTex);
    c.setScale(0.08f, 0.08f);

    float x = rand() % (screen.x - 50) + 25;
    float y = rand() % (screen.y - 50) + 25;

    c.setPosition(x, y);
    cheeses.push_back(c);

    cheeseVel.push_back({
        (float)(rand() % 3 - 1) * 1.5f,
        (float)(rand() % 3 - 1) * 1.5f
        });
}

void Game::spawnBadCheese()
{
    sf::Sprite b(badCheeseTex);
    b.setScale(0.08f, 0.08f);

    float x = rand() % (screen.x - 50) + 25;
    float y = rand() % (screen.y - 50) + 25;

    b.setPosition(x, y);
    badCheeses.push_back(b);
}

void Game::updateBigEater()
{
    float speed = 6.5f * speedMultiplier;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  mousePos.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) mousePos.x += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    mousePos.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  mousePos.y += speed;

    mousePos.x = std::max(0.f, std::min((float)screen.x - mouse.getGlobalBounds().width, mousePos.x));
    mousePos.y = std::max(0.f, std::min((float)screen.y - mouse.getGlobalBounds().height, mousePos.y));

    mouse.setPosition(mousePos);

    if (score >= 52 && !upgraded)
    {
        sf::FloatRect old = mouse.getGlobalBounds();
        mouse.setTexture(mousePlusTex, true);

        sf::Vector2u ns = mousePlusTex.getSize();
        mouse.setScale(old.width / ns.x, old.height / ns.y);

        speedMultiplier = 1.1f;
        upgraded = true;
    }

    sf::Vector2f dir = mousePos - catPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len != 0) dir /= len;

    catPos += dir * 3.5f;

    catPos.x = std::max(0.f, std::min((float)screen.x - cat.getGlobalBounds().width, catPos.x));
    catPos.y = std::max(0.f, std::min((float)screen.y - cat.getGlobalBounds().height, catPos.y));

    cat.setPosition(catPos);

    for (size_t i = 0; i < cheeses.size(); i++)
    {
        cheeses[i].move(cheeseVel[i]);

        auto p = cheeses[i].getPosition();
        if (p.x < 0 || p.x > screen.x - 20) cheeseVel[i].x *= -1;
        if (p.y < 0 || p.y > screen.y - 20) cheeseVel[i].y *= -1;
    }

    for (size_t i = 0; i < cheeses.size(); )
    {
        if (mouse.getGlobalBounds().intersects(cheeses[i].getGlobalBounds()))
        {
            score++;
            cheeses.erase(cheeses.begin() + i);
            cheeseVel.erase(cheeseVel.begin() + i);
            spawnCheese();
        }
        else i++;
    }

    for (auto& b : badCheeses)
    {
        if (mouse.getGlobalBounds().intersects(b.getGlobalBounds()))
        {
            finalScore = score;
            state = GameOver;
        }
    }

    if (mouse.getGlobalBounds().intersects(cat.getGlobalBounds()))
    {
        finalScore = score;
        state = GameOver;
    }
}