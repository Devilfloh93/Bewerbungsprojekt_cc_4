#include "Game.h"
#include "Gui.h"
#include "Item.h"
#include "Player.h"
#include "Surface.h"
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
    std::vector<std::unique_ptr<World>> world;
    std::vector<std::unique_ptr<Surface>> surfaces;
    std::vector<std::unique_ptr<Title>> titles;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<AllItems>> allItems;
    std::vector<std::unique_ptr<Item>> items;

    // Fonts
    sf::Font font01;
    font01.loadFromFile("ressources/font/Round9x13.ttf");

    // Textures
    sf::Texture btnTexture01;
    btnTexture01.loadFromFile("ressources/textures/Buttons_all_01.png");

    sf::Texture surfaceTexture01;
    surfaceTexture01.loadFromFile("ressources/textures/TileSet_V2.png");

    sf::Texture playerTexture01;
    playerTexture01.loadFromFile("ressources/textures/Human-Worker-Red.png");

    // Player Init
    auto player = Player("PlayerName", PlayerSurvivalStats{100.0F, 100.0F, 100.0F}, 1.0F);
    sf::Sprite playerSprite;

    Menu::Init(game, font01, btnTexture01, titles, buttons);
    player.Init(playerSprite, playerTexture01);
    InitSurface(surfaces, game, surfaceTexture01);
    InitWorld(world);
    InitItems(allItems);
    game.InitView(view);

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
                        player.SetMovement(PlayerMove::Left);
                        break;
                    case sf::Keyboard::Key::D:
                        player.SetMovement(PlayerMove::Right);
                        break;
                    case sf::Keyboard::Key::W:
                        player.SetMovement(PlayerMove::Up);
                        break;
                    case sf::Keyboard::Key::S:
                        player.SetMovement(PlayerMove::Down);
                        break;
                    case sf::Keyboard::Key::P:
                        for (const auto &data : allItems)
                        {
                            sf::Sprite itemSprite;
                            auto texture = data->GetTexture();
                            auto textureCoords = data->GetTextureCoords();
                            auto itemID = data->GetID();
                            auto itemName = data->GetName();

                            itemSprite.setTexture(*texture);
                            itemSprite.setTextureRect(textureCoords);
                            itemSprite.setPosition(50, 100);

                            items.push_back(std::make_unique<Item>(itemSprite, itemID, itemName));
                        }

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
                        player.SetMovement(PlayerMove::NotMoving);
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

            DrawSurface(window, surfaces, player, playerSprite, game);

            player.HandleMovement(clock, playerSprite, game, world);
            window.draw(playerSprite);

            DrawWorld(window, world);
            DrawItems(window, items);
        }

        switch (menuState)
        {
        case MenuState::Pause:
            Menu::Draw(window, menuView, titles, buttons, MenuState::Pause);
            break;
        case MenuState::Options:
            Menu::Draw(window, menuView, titles, buttons, MenuState::Options);
            break;
        case MenuState::Main:
            Menu::Draw(window, menuView, titles, buttons, MenuState::Main);
            break;
        case MenuState::Inventory:
            Menu::Draw(window, menuView, titles, buttons, MenuState::Inventory);
            break;

        default:
            break;
        }

        window.display();
    }

    return 0;
}
