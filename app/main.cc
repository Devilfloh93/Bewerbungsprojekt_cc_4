#include "Font.h"
#include "Game.h"
#include "Gui.h"
#include "Item.h"
#include "Player.h"
#include "Surface.h"
#include "Texture.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    std::uint16_t windowWidth = 1280U;
    std::uint16_t windowHeight = 720U;
    //  std::uint16_t windowWidth = sf::VideoMode::getDesktopMode().width;
    //  std::uint16_t windowHeight = sf::VideoMode::getDesktopMode().height;

    // Init Game
    auto game = Game(windowWidth, windowHeight);
    auto gameWidth = game.GetGameWidth();
    auto gameHeight = game.GetGameHeight();

    //Init Views
    auto defaultCenter = sf::Vector2f(windowWidth / 2U, windowHeight / 2U);

    sf::View menuView;
    menuView.setSize(sf::Vector2f(windowWidth, windowHeight));
    menuView.setCenter(defaultCenter);

    auto view = sf::View(defaultCenter, sf::Vector2f(windowWidth, windowHeight));

    // Init Window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    // Clocks
    sf::Clock clock;

    // Vector
    std::vector<std::unique_ptr<ItemCfg>> itemCfg;
    std::vector<std::unique_ptr<World>> world;
    std::vector<std::unique_ptr<Surface>> surfaces;
    std::vector<std::unique_ptr<Title>> titles;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<Font>> fonts;

    // Texture Init
    InitTexture(textures);
    std::cout << "Texture Init Done!" << std::endl;

    // Font Init
    InitFont(fonts);
    std::cout << "Font Init Done!" << std::endl;

    // Player Init
    auto player = InitPlayer(textures);
    std::cout << "Player Init Done!" << std::endl;

    // Menu Init
    Menu::Init(game, titles, buttons, textures, fonts);
    std::cout << "Menu Init Done!" << std::endl;

    // Surface Init
    InitSurface(surfaces, game, textures);
    std::cout << "Surface Init Done!" << std::endl;

    // World Init
    InitWorld(world, textures);
    std::cout << "World Init Done!" << std::endl;

    // Item Init
    InitItemCfg(itemCfg, textures);
    std::cout << "Item Init Done!" << std::endl;

    // View Init
    game.InitView(view);
    std::cout << "View Init Done!" << std::endl;

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
                if (game.GetMenuState() != MenuState::Playing)
                    breakLoop = Menu::HandleBtnClicked(window, buttons, state, game);
                else if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                    game.HandleViewPosition(window, view);
                break;
            case sf::Event::KeyPressed:
                if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                {
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
                        player.UseItem(world, itemCfg, items);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case sf::Event::KeyReleased:
                if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
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
                if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                {
                    if (event.mouseWheelScroll.delta > 0U)
                    {
                        if (game.GetZoom() < game.GetMaxZoom())
                        {
                            view.zoom(0.5F);
                            game.SetZoom(1U);
                        }
                    }
                    else
                    {
                        if (game.GetZoom() > 0U)
                        {
                            view.zoom(2.0F);
                            game.SetZoom(-1);
                        }
                    }

                    game.UpdateView(view);
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
            window.setView(view);

            DrawSurface(window, surfaces, player, game);

            player.HandleMove(clock, game, world, items);

            DrawItems(window, items);
            window.draw(*(player.GetSprite()));
            DrawWorld(window, world);
        }

        switch (menuState)
        {
        case MenuState::Pause:
            Menu::Draw(window, menuView, titles, buttons, menuState);
            break;
        case MenuState::Options:
            Menu::Draw(window, menuView, titles, buttons, menuState);
            break;
        case MenuState::Main:
            Menu::Draw(window, menuView, titles, buttons, menuState);
            break;
        case MenuState::Inventory:
            Menu::Draw(window, menuView, titles, buttons, menuState, player, itemCfg);
            break;

        default:
            break;
        }

        window.display();
    }

    return 0;
}
