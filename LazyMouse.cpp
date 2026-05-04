#include "LazyMouse.h"
#include <ctime>
#include <algorithm>
#include <iostream>

// ===============================
void LazyMouse::init(sf::RenderWindow& window)
{
    phoneTex.loadFromFile("D:/VS project/鼠鼠大作战/images/phone.png");
    phonePlusTex.loadFromFile("D:/VS project/鼠鼠大作战/images/phone+.png");
    phonePlusPlusTex.loadFromFile("D:/VS project/鼠鼠大作战/images/phone++.png");
    sleepTex.loadFromFile("D:/VS project/鼠鼠大作战/images/sleep.png");

    cheeseTex.loadFromFile("D:/VS project/鼠鼠大作战/images/cheese.png");
    badCheeseTex.loadFromFile("D:/VS project/鼠鼠大作战/images/badcheese.png");

    sprite.setTexture(sleepTex);

    sf::Vector2u ws = window.getSize();
    sf::Vector2u ts = sleepTex.getSize();

    float scale = std::max((float)ws.x / ts.x, (float)ws.y / ts.y);

    sprite.setScale(scale, scale);

    float sw = ts.x * scale;
    float sh = ts.y * scale;

    sprite.setPosition((ws.x - sw) / 2.f, (ws.y - sh) / 2.f - 60.f);

    float btnScale = 0.1f;

    cheeseButton.setTexture(cheeseTex);
    cheeseButton.setScale(btnScale, btnScale);

    sf::Vector2u g = cheeseTex.getSize();

    cheeseButton.setPosition(
        ws.x - g.x * btnScale - 20.f,
        ws.y - g.y * btnScale - 20.f
    );

    badCheeseButton.setTexture(badCheeseTex);

    sf::Vector2u b = badCheeseTex.getSize();

    badCheeseButton.setScale(
        (g.x * btnScale) / b.x,
        (g.y * btnScale) / b.y
    );

    badCheeseButton.setPosition(
        cheeseButton.getPosition().x - b.x * badCheeseButton.getScale().x - 15.f,
        cheeseButton.getPosition().y
    );

    backBtn.setSize({ 120.f, 40.f });
    backBtn.setFillColor(sf::Color(120, 120, 120));
    backBtn.setPosition(10.f, 10.f);

    font.loadFromFile("D:/VS project/鼠鼠大作战/x64/Debug/simhei.ttf");

    backText.setFont(font);
    backText.setString("MENU");
    backText.setCharacterSize(20);
    backText.setPosition(30.f, 15.f);

    clockText.setFont(font);
    clockText.setCharacterSize(30);
    clockText.setPosition(10.f, 60.f);

    mood = rand() % 101;

    moodText.setFont(font);
    moodText.setCharacterSize(30);
    moodText.setPosition(10.f, 100.f);

    bgState = 0;
    mouseDownLast = false;
    lockBg = false;
}

// ===============================
void LazyMouse::update(sf::RenderWindow& window)
{
    std::time_t now = std::time(nullptr);
    std::tm t;

#ifdef _WIN32
    localtime_s(&t, &now);
#else
    localtime_r(&now, &t);
#endif

    char buf[32];
    sprintf_s(buf, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    clockText.setString(buf);

    if (!lockBg)
    {
        if (t.tm_hour >= 12 || t.tm_hour < 2)
            sprite.setTexture(phoneTex);
        else
            sprite.setTexture(sleepTex);
    }

    if (bgState == 1)
        sprite.setTexture(phonePlusTex);
    else if (bgState == 2)
        sprite.setTexture(phonePlusPlusTex);

    bool mouseNow = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (mouseNow && !mouseDownLast)
    {
        if (backBtn.getGlobalBounds().contains(m))
        {
            if (state)
                *state = GameState::MainMenu;
        }

        if (cheeseButton.getGlobalBounds().contains(m))
        {
            mood = std::min(mood + 5, 100);
            bgState = 1;
            lockBg = true;
        }

        if (badCheeseButton.getGlobalBounds().contains(m))
        {
            mood = std::max(mood - 5, 0);
            bgState = 2;
            lockBg = true;
        }
    }

    mouseDownLast = mouseNow;

    moodText.setString("Mood: " + std::to_string(mood));
}

// ===============================
void LazyMouse::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(clockText);
    window.draw(moodText);

    window.draw(cheeseButton);
    window.draw(badCheeseButton);

    window.draw(backBtn);
    window.draw(backText);
}

// ===============================
// ⭐新增：MENU按钮点击检测
bool LazyMouse::isBackClicked(sf::Vector2f mousePos) const
{
    return backBtn.getGlobalBounds().contains(mousePos);
}