
#include "EventHandler.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <format>
#include <iostream>

using namespace std;

int main()
{
    uint16_t windowWidth = 1280U;
    uint16_t windowHeight = 720U;
    //  uint16_t windowWidth = sf::VideoMode::getDesktopMode().width;
    //  uint16_t windowHeight = sf::VideoMode::getDesktopMode().height;

    // Init Game
    auto game = Game(windowWidth, windowHeight);
    auto player = game.GetPlayer();

    // Init Window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    sf::Clock clock;

    // EventHandler Init
    auto eventHandler = EventHandler();

    // Folder Structur Init
    game.CreateFolder();

    // Anim Init
    game.InitAnim();
    cout << "Anim Init Done!" << endl;

    // View Init
    game.InitViews();
    cout << "View Init Done!" << endl;

    // Texture Init
    game.InitTexture();
    cout << "Texture Init Done!" << endl;

    // Font Init
    game.InitFont();
    cout << "Font Init Done!" << endl;

    // Menu Init
    game.InitMenu();
    cout << "Menu Init Done!" << endl;

    // Surface Init
    game.InitSurface();
    cout << "Surface Init Done!" << endl;

    // World Init
    game.InitWorld();
    cout << "World Init Done!" << endl;

    // Item Init
    game.InitItemCfg();
    cout << "Item Init Done!" << endl;

    // Draw Stats Init
    game.InitDrawStats();
    cout << "DrawStats Init Done!" << endl;

    while (window.isOpen())
    {
        eventHandler.ResetBreak();
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                eventHandler.Quit(window);
                break;
            case sf::Event::MouseButtonPressed:
                eventHandler.MouseBtnPressed(window, game);
                break;
            case sf::Event::KeyPressed:
                eventHandler.KeyPressed(player, game, event.key.code);
                break;
            case sf::Event::TextEntered:
                eventHandler.TxtEntered(game, event.text.unicode);
                break;
            case sf::Event::KeyReleased:
                eventHandler.KeyReleased(game, player);
                break;
            case sf::Event::MouseWheelScrolled:
                eventHandler.MouseWheelScrolled(game, event.mouseWheelScroll.delta);
                break;
            default:
                break;
            }

            if (eventHandler.GetBreak())
                break;
        }

        player = game.GetPlayer();
        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
        {
            auto gameView = game.GetView();
            auto playerSprite = player->GetSprite();

            window.setView(gameView);

            game.DrawSurface(window, player);
            player->HandleMove(clock, game);

            game.DrawItems(window);
            window.draw(*playerSprite);
            game.DrawWorld(window);

            player->DrawStats(window, game);
        }
        else
        {
            game.DrawMenu(window);
        }

        window.display();
    }

    game.GetPlayer()->Save();
    game.GetThread()->Join();
    return 0;
}
