#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include "BigEater.h"
#include "Parkour.h"
#include "LazyMouse.h"  

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Mouse Adventure", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    Game game;
    game.init(window);

    ParkourGame parkour;
    parkour.state = &game.state;
    parkour.init(window);

    LazyMouse lazy;
    lazy.init(window);

    sf::Vector2u screen = window.getSize();
    float cx = screen.x / 2.f;
    float cy = screen.y / 2.f;

    sf::RectangleShape button1({ 260, 60 });
    sf::RectangleShape button2({ 260, 60 });
    sf::RectangleShape button3({ 260, 60 });

    button1.setOrigin(130, 30);
    button2.setOrigin(130, 30);
    button3.setOrigin(130, 30);

    button1.setPosition(cx, cy - 100);
    button2.setPosition(cx, cy);
    button3.setPosition(cx, cy + 100);

    button1.setFillColor(sf::Color::White);
    button2.setFillColor(sf::Color::White);
    button3.setFillColor(sf::Color::White);

    sf::Text t1("Big Eater", game.font, 28);
    sf::Text t2("Parkour Mouse", game.font, 28);
    sf::Text t3("Lazy Mouse", game.font, 28);

    auto center = [&](sf::Text& t, sf::Vector2f p)
        {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.width / 2.f, b.height / 2.f);
            t.setPosition(p);
            t.setFillColor(sf::Color::Black);
        };

    center(t1, button1.getPosition());
    center(t2, button2.getPosition());
    center(t3, button3.getPosition());

    sf::RectangleShape quit({ 160, 45 });
    quit.setPosition(screen.x - 180, 20);
    quit.setFillColor(sf::Color(180, 50, 50));

    sf::Text quitText("EXIT", game.font, 20);
    quitText.setPosition(screen.x - 135, 28);

    GameState lastMode = MainMenu;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            sf::Vector2f mpos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (button1.getGlobalBounds().contains(mpos))
                button1.setFillColor(sf::Color(200, 200, 255));
            else
                button1.setFillColor(sf::Color::White);

            if (button2.getGlobalBounds().contains(mpos))
                button2.setFillColor(sf::Color(200, 200, 255));
            else
                button2.setFillColor(sf::Color::White);

            if (button3.getGlobalBounds().contains(mpos))
                button3.setFillColor(sf::Color(200, 200, 255));
            else
                button3.setFillColor(sf::Color::White);

            if (event.type == sf::Event::MouseButtonPressed &&
                quit.getGlobalBounds().contains(mpos))
            {
                window.close();
            }

            // ================= 菜单点击 =================
            if (game.state == MainMenu && event.type == sf::Event::MouseButtonPressed)
            {
                if (button1.getGlobalBounds().contains(mpos))
                {
                    game.state = BigEater;
                    lastMode = BigEater;
                    game.startBigEater();
                }

                if (button2.getGlobalBounds().contains(mpos))
                {
                    game.state = Parkour;
                    lastMode = Parkour;
                    parkour.start();
                }

                if (button3.getGlobalBounds().contains(mpos))
                {
                    game.state = LazyMouseMode;   // ✔关键修复
                    lastMode = LazyMouseMode;
                }
            }

            if (game.state == GameOver && event.type == sf::Event::MouseButtonPressed)
            {
                game.state = MainMenu;
                game.resetToMenu();
            }
        }

        // ================= UPDATE =================
        if (game.state == BigEater)
        {
            game.updateBigEater();

            if (game.state == GameOver)
                game.finalScore = game.score;
        }

        if (game.state == Parkour)
        {
            parkour.update(window);

            if (game.state == GameOver)
                game.finalScore = parkour.finalScore;
        }

        // ⭐⭐⭐ 新增：LazyMouse update（关键修复）
        if (game.state == LazyMouseMode)
        {
            lazy.update(window);
        }

        window.clear();
        window.draw(game.background);

        // ================= DRAW =================
        if (game.state == MainMenu)
        {
            window.draw(button1);
            window.draw(button2);
            window.draw(button3);

            window.draw(t1);
            window.draw(t2);
            window.draw(t3);
        }
        else if (game.state == BigEater)
        {
            for (auto& c : game.cheeses) window.draw(c);
            for (auto& b : game.badCheeses) window.draw(b);

            window.draw(game.cat);
            window.draw(game.mouse);

            sf::Text s("Score: " + std::to_string(game.score), game.font, 30);
            s.setPosition(10, 10);
            window.draw(s);
        }
        else if (game.state == Parkour)
        {
            parkour.draw(window);
        }
        else if (game.state == LazyMouseMode)
        {
            // ⭐⭐⭐ 新增：LazyMouse draw（关键修复）
            lazy.draw(window);

            sf::Vector2f mpos =
                window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (lazy.isBackClicked(mpos))
                {
                    game.state = MainMenu;
                }
            }
        }
        else if (game.state == GameOver)
        {
            sf::Text over("GAME OVER", game.font, 60);
            over.setPosition(cx - 150, cy - 200);

            sf::Text sc("Final Score: " + std::to_string((int)game.finalScore), game.font, 40);
            sc.setPosition(cx - 160, cy - 50);

            sf::Text hint("Click to return menu", game.font, 25);
            hint.setPosition(cx - 150, cy + 80);

            window.draw(over);
            window.draw(sc);
            window.draw(hint);
        }

        window.draw(quit);
        window.draw(quitText);

        window.display();
    }

    return 0;
}