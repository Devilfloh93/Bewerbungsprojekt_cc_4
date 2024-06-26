
#include "EventHandler.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <format>
#include <iostream>

using namespace std;

int main()
{
    // Init Game
    auto game = Game();

    // EventHandler Init
    EventHandler eventHandler;

    game.Init();
    auto window = game.GetWindow();

    while (game.GetWindow()->isOpen())
    {
        eventHandler.ResetBreak();

        for (auto event = sf::Event{}; window->pollEvent(event);)
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                eventHandler.Quit(game);
                break;
            case sf::Event::MouseButtonPressed:
                eventHandler.MousePressed(game);
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

            /**
             * @brief break out when we change the Menu or go Ingame if needed to avoid double presses in submenus or ingame interactions
             *
             */
            if (eventHandler.GetBreak())
                break;
        }
        window = game.GetWindow();

        window->clear(sf::Color(50U, 50U, 50U));

        /**
         * @brief While the Game is running the Render() will be executed and while we are in a Menu the RenderMenu() will be executed
         *
         */
        if (game.GetPlaying() && game.GetMenuState().first == MenuState::Playing)
            game.Render();
        else
            game.RenderMenu();

        window->display();
    }

    game.Saving(true);
    game.GetThread()->Join();
    return 0;
}
