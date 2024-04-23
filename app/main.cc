
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

    // EventHandler Init
    EventHandler eventHandler;

    // Init Window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    sf::Clock clock;


    game.Init();

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
                eventHandler.KeyPressed(game, event.key.code);
                break;
            case sf::Event::TextEntered:
                eventHandler.TxtEntered(game, event.text.unicode);
                break;
            case sf::Event::KeyReleased:
                eventHandler.KeyReleased(game);
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

        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
        {
            game.Render(window, clock);
        }
        else
        {
            game.RenderMenu(window);
        }

        window.display();
    }

    game.Saving(true);
    game.GetThread()->Join();
    return 0;
}
