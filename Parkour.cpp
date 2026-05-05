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
bool ParkourGame::isGap(float x)
{
    int seg = (int)(x / 200.f);
    return (seg % 8 == 0);
}

// ===============================
float ParkourGame::getGroundHeight(float x)
{
    if (isGap(x))
        return -1.f;

    return 520.f +
        std::sin(x * 0.01f) * 60.f +
        std::sin(x * 0.003f) * 80.f;
}

// ===============================
void ParkourGame::init(sf::RenderWindow& window)
{
<<<<<<< HEAD
    if (!mouseTex.loadFromFile("D:/VS project/鼠鼠大作战/images/mouse.png"))
        std::cout << "mouse.png load failed!" << std::endl;

    if (!mousePlusPlusTex.loadFromFile("D:/VS project/鼠鼠大作战/images/mouse++.png"))
=======
    if (!mouseTex.loadFromFile("images/mouse.png"))
        std::cout << "mouse.png load failed!" << std::endl;

    if (!mousePlusPlusTex.loadFromFile("images/mouse++.png"))
>>>>>>> afa7661 (temp: save local changes before rebase)
        std::cout << "mouse++.png load failed!" << std::endl;

    if (!cheeseTex.loadFromFile("images/cheese.png"))
        std::cout << "cheese.png load failed!" << std::endl;

<<<<<<< HEAD
    if (!font.loadFromFile("D:/VS project/鼠鼠大作战/x64/Debug/simhei.ttf"))
=======
    if (!font.loadFromFile("fonts/simhei.ttf"))
>>>>>>> afa7661 (temp: save local changes before rebase)
        std::cout << "font load failed!" << std::endl;

    mouse.setTexture(mouseTex);

    float targetW = 80.f;
    float targetH = 80.f;

    auto ts = mouseTex.getSize();
    mouse.setScale(targetW / ts.x, targetH / ts.y);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);

    start();
}

// ===============================
void ParkourGame::start()
{
    cameraX = 0.f;
    score = 0.f;
    finalScore = 0.f;

    velocity = { 0.f, 0.f };
    onGround = false;
    isFalling = false;

    resetCheese();
    lastCheeseX = 0.f;

    float g = getGroundHeight(200.f);
    mouse.setPosition(200.f, g - mouse.getGlobalBounds().height);

    onGround = true;
}

// ===============================
void ParkourGame::reset()
{
    start();
}

// ===============================
void ParkourGame::generateCheeseChain(float baseX)
{
    int pattern[3] = { 3, 4, 5 };
    int count = pattern[rand() % 3];

    float spacing = 55.f;
    float startOffset = -(count / 2.f) * spacing;

<<<<<<< HEAD
    // ⭐关键：老鼠高度作为统一贴地参考
    float mouseHeight = mouse.getGlobalBounds().height;
    float cheeseH = cheeseTex.getSize().y * 0.06f;

    // 固定抬升：保证可吃且不贴地
=======
    float mouseHeight = mouse.getGlobalBounds().height;
    float cheeseH = cheeseTex.getSize().y * 0.06f;

>>>>>>> afa7661 (temp: save local changes before rebase)
    float heightOffset = mouseHeight * 0.6f;

    for (int i = 0; i < count; i++)
    {
        float x = baseX + startOffset + i * spacing;

        float g = getGroundHeight(x);

<<<<<<< HEAD
        // 空洞直接跳过
        if (g < 0.f)
            continue;

        // ================= ⭐核心逻辑：贴地生成 =================
        float y = g - heightOffset - cheeseH;
        // =======================================================
=======
        if (g < 0.f)
            continue;

        float y = g - heightOffset - cheeseH;
>>>>>>> afa7661 (temp: save local changes before rebase)

        sf::Sprite c;
        c.setTexture(cheeseTex);
        c.setScale(0.06f, 0.06f);
        c.setPosition(x, y);

        cheeses.push_back(c);
    }
}

// ===============================
void ParkourGame::update(sf::RenderWindow& window)
{
    static GameState lastState = GameOver;

    if (*state == Parkour && lastState != Parkour)
        start();

    lastState = *state;

    if (*state == GameOver)
        return;

    float speed = 6.f;
    float screenX = window.getSize().x / 2.f;

    float oldCam = cameraX;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        cameraX += speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        cameraX -= speed;

    if (cameraX < 0.f)
        cameraX = 0.f;

    sf::Texture* tex = nullptr;

    if (score >= 52)
        tex = &mousePlusPlusTex;
    else
        tex = &mouseTex;

    mouse.setTexture(*tex);

    float targetW = 80.f;
    float targetH = 80.f;

    auto ts = tex->getSize();
    if (ts.x > 0 && ts.y > 0)
        mouse.setScale(targetW / ts.x, targetH / ts.y);

    if (cameraX - lastCheeseX > 300.f)
    {
        generateCheeseChain(cameraX + 700.f);
        lastCheeseX = cameraX;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onGround)
    {
        velocity.y = jumpPower;
        onGround = false;
    }

    velocity.y += gravity;
    mouse.move(0.f, velocity.y);
    mouse.setPosition(screenX, mouse.getPosition().y);

    float worldX = cameraX + screenX;
    float groundY = getGroundHeight(worldX);
    float nextGroundY = getGroundHeight(worldX + 10.f);

    bool hasGround = (groundY >= 0.f);
    bool hasRightGround = (nextGroundY >= 0.f);

    float h = mouse.getGlobalBounds().height;
    float footY = mouse.getPosition().y + h;

    onGround = false;

    if (!isFalling && hasGround && velocity.y >= 0.f)
    {
        if (footY >= groundY)
        {
            mouse.setPosition(screenX, groundY - h);
            velocity.y = 0.f;
            onGround = true;
        }
    }

    if (!isFalling && hasGround && !hasRightGround && velocity.y >= 0.f)
        isFalling = true;

    if (isFalling &&
        mouse.getPosition().y > window.getSize().y + 120.f)
    {
        finalScore = score;
        *state = GameOver;
        return;
    }

    for (int i = 0; i < cheeses.size(); )
    {
        sf::FloatRect player = mouse.getGlobalBounds();
        player.height *= 0.7f;

        if (player.intersects(cheeses[i].getGlobalBounds()))
        {
            score += 1.f;
            cheeses.erase(cheeses.begin() + i);
        }
        else i++;
    }

    float delta = cameraX - oldCam;

    for (auto& c : cheeses)
        c.move(-delta, 0.f);
}

// ===============================
void ParkourGame::draw(sf::RenderWindow& window)
{
    scoreText.setString("Score: " + std::to_string((int)score));
    window.draw(scoreText);

    if (*state == GameOver)
    {
        gameOverText.setString(
            "Game Over\nFinal Score: " + std::to_string((int)finalScore)
        );

        gameOverText.setPosition(
            window.getSize().x / 2.f - 150.f,
            window.getSize().y / 2.f - 100.f
        );

        window.draw(gameOverText);
        return;
    }

    for (int i = 0; i < window.getSize().x; i += 4)
    {
        float x = cameraX + i;
        float y = getGroundHeight(x);

        if (y < 0.f) continue;

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i, y), sf::Color(139, 69, 19)),
            sf::Vertex(sf::Vector2f(i, window.getSize().y), sf::Color(139, 69, 19))
        };

        window.draw(line, 2, sf::Lines);
    }

    for (auto& c : cheeses)
        window.draw(c);

    window.draw(mouse);
}