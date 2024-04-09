
#include "Game.h"
#include "Thread.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
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

    // Init Window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    sf::Clock clock;

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

    // Player Init
    auto player = game.InitPlayer();
    cout << "Player Init Done!" << endl;

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

    // Thread Init
    auto thread = Thread(window, player, game);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            bool breakLoop = false;
            auto state = game.GetMenuState();
            switch (event.type)
            {
            case sf::Event::Closed:
                game.Quit(window);
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
                            breakLoop = true;
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
                    game.UpdateZoom(event.mouseWheelScroll.delta);
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
            auto gameView = game.GetView();
            auto playerSprite = *(player.GetSprite());

            window.setView(gameView);

            game.DrawSurface(window, player);
            player.HandleMove(clock, game);

            game.DrawItems(window);
            window.draw(playerSprite);
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

    player.Save();
    thread.Join();
    return 0;
}
