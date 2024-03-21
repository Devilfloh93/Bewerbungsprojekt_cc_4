
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

int main()
{
    std::uint16_t windowWidth = 1280U;
    std::uint16_t windowHeight = 720U;
    //  std::uint16_t windowWidth = sf::VideoMode::getDesktopMode().width;
    //  std::uint16_t windowHeight = sf::VideoMode::getDesktopMode().height;

    // Init Game
    auto game = Game(windowWidth, windowHeight);

    // Init Window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    // Clocks
    sf::Clock clock;

    // View Init
    game.InitViews();
    std::cout << "View Init Done!" << std::endl;

    // Texture Init
    game.InitTexture();
    std::cout << "Texture Init Done!" << std::endl;

    // Font Init
    game.InitFont();
    std::cout << "Font Init Done!" << std::endl;

    // Player Init
    auto player = game.InitPlayer();
    std::cout << "Player Init Done!" << std::endl;

    // Menu Init
    game.InitMenu();
    std::cout << "Menu Init Done!" << std::endl;

    // Surface Init
    game.InitSurface();
    std::cout << "Surface Init Done!" << std::endl;

    // World Init
    game.InitWorld();
    std::cout << "World Init Done!" << std::endl;

    // Item Init
    game.InitItemCfg();
    std::cout << "Item Init Done!" << std::endl;


    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            bool breakLoop = false;
            auto state = game.GetMenuState();
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                breakLoop = true;
                break;
            case sf::Event::MouseButtonPressed:
                if (state != MenuState::Playing)
                    breakLoop = game.HandleBtnClicked(window);
                else if (game.GetPlaying() && state == MenuState::Playing)
                    game.HandleViewPosition(window);
                break;
            case sf::Event::KeyPressed:
                if (game.GetPlaying())
                {
                    switch (state)
                    {
                    case MenuState::Playing:
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Key::Escape:
                            game.SetMenuState(MenuState::Pause);
                            breakLoop = true;
                            break;
                        case sf::Keyboard::Key::I:
                            game.SetMenuState(MenuState::Inventory);
                            breakLoop = true;
                            break;
                        case sf::Keyboard::Key::A:
                            player.SetMove(PlayerMove::Left);
                            break;
                        case sf::Keyboard::Key::D:
                            player.SetMove(PlayerMove::Right);
                            break;
                        case sf::Keyboard::Key::W:
                            player.SetMove(PlayerMove::Up);
                            break;
                        case sf::Keyboard::Key::S:
                            player.SetMove(PlayerMove::Down);
                            break;
                        case sf::Keyboard::Key::P:
                            player.UseItem(game);
                            break;
                        default:
                            break;
                        }
                        break;
                    case MenuState::Inventory:
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Key::I:
                            game.SetMenuState(MenuState::Playing);
                            breakLoop = true;
                            break;
                        default:
                            break;
                        }
                        break;
                    case MenuState::Pause:
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Key::Escape:
                            game.SetMenuState(MenuState::Playing);
                            breakLoop = true;
                            break;
                        default:
                            break;
                        }
                        break;
                    case MenuState::Options:
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Key::Escape:
                            game.SetMenuState(MenuState::Pause);
                            break;
                        default:
                            break;
                        }
                        break;

                    default:
                        break;
                    }
                }
                break;
            case sf::Event::KeyReleased:
                if (game.GetPlaying() && state == MenuState::Playing)
                {
                    bool keyPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S));

                    if (!keyPressed)
                        player.SetMove(PlayerMove::NotMoving);
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (game.GetPlaying() && state == MenuState::Playing)
                {
                    if (event.mouseWheelScroll.delta > 0U)
                    {
                        if (game.GetZoom() < game.GetMaxZoom())
                        {
                            game.SetZoom(1U, 0.5F);
                        }
                    }
                    else
                    {
                        if (game.GetZoom() > 0U)
                        {
                            game.SetZoom(1U, 2.0F);
                        }
                    }

                    game.UpdateView();
                }
                break;
            default:
                break;
            }

            if (breakLoop)
                break;
        }

        auto menuState = game.GetMenuState();
        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetPlaying() && menuState == MenuState::Playing)
        {
            window.setView(game.GetView());

            game.DrawSurface(window, player);

            player.HandleMove(clock, game);

            game.DrawItems(window);
            window.draw(*(player.GetSprite()));
            game.DrawWorld(window);
        }

        switch (menuState)
        {
        case MenuState::Pause:
            game.DrawMenu(window);
            break;
        case MenuState::Options:
            game.DrawMenu(window);
            break;
        case MenuState::Main:
            game.DrawMenu(window);
            break;
        case MenuState::Inventory:
            game.DrawMenu(window, player);
            break;

        default:
            break;
        }

        window.display();
    }

    return 0;
}
