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

    auto game = Game(windowWidth, windowHeight);
    auto gameWidth = game.GetGameWidth();
    auto gameHeight = game.GetGameHeight();

    auto defaultCenter = sf::Vector2f(windowWidth / 2U, windowHeight / 2U);
    sf::View menuView;
    menuView.setSize(sf::Vector2f(windowWidth, windowHeight));
    menuView.setCenter(defaultCenter);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);
    auto view = std::make_unique<sf::View>(defaultCenter, sf::Vector2f(windowWidth, windowHeight));

    sf::Clock clock;

    // Vector
    std::vector<std::unique_ptr<World>> world;
    std::vector<std::unique_ptr<Surface>> surfaces;
    std::vector<std::unique_ptr<Title>> titles;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<Item>> allItems;

    // Fonts
    sf::Font font01;
    font01.loadFromFile("ressources/font/Round9x13.ttf");

    //Textures
    sf::Texture btnTexture01;
    btnTexture01.loadFromFile("ressources/textures/Buttons_all_01.png");

    sf::Texture surfaceTexture01;
    surfaceTexture01.loadFromFile("ressources/textures/TileSet_V2.png");

    sf::Texture worldTexture01;
    worldTexture01.loadFromFile("ressources/textures/Premium_TreesUpdated_No_Outline.png");

    sf::Texture playerTexture01;
    playerTexture01.loadFromFile("ressources/textures/Human-Worker-Red.png");

    //Player Init
    auto player = Player("PlayerName", PlayerSurvivalStats{100.0F, 100.0F, 100.0F}, 1.0F);
    sf::Sprite playerSprite;

    InitMenus(game, font01, btnTexture01, titles, buttons);
    InitPlayer(playerSprite, playerTexture01);
    InitSurface(surfaces, game, surfaceTexture01);
    InitWorld(world, worldTexture01);
    InitItems(allItems);
    auto [viewCenter, viewSize] = InitView(game, *view);

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
                {
                    // get the current mouse position in the window
                    auto pixelPos = sf::Mouse::getPosition(window);
                    // convert it to world coordinates
                    auto worldPos = window.mapPixelToCoords(pixelPos);
                    for (const auto &data : buttons)
                    {
                        auto btnPos = data->GetSprite().getPosition();
                        auto btnLSize = data->GetSprite().getLocalBounds().getSize();
                        auto btnScale = data->GetSprite().getScale();
                        auto data1 = data->GetMenuState();

                        bool clickBtn = false;
                        for (const auto &data2 : data1)
                        {
                            if (state == data2)
                            {
                                clickBtn = true;
                                break;
                            }
                        }

                        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) &&
                            worldPos.y > btnPos.y && worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && clickBtn)
                        {
                            switch (data->GetBtnFnc())
                            {
                            case BtnFunc::Play:
                                game.SetPlaying(true);
                                game.SetMenuState(MenuState::Playing);
                                breakLoop = true;
                                break;
                            case BtnFunc::Resume:
                                game.SetMenuState(MenuState::Playing);
                                breakLoop = true;
                                break;
                            case BtnFunc::Quit:
                                window.close();
                                breakLoop = true;
                                break;
                            case BtnFunc::Options:
                                game.SetMenuState(MenuState::Options);
                                /*
                                    TODO: ADD OPTIONS TO OPTION MENU
                                */
                                // game.SetWindowWidth(1920U);
                                // game.SetWindowHeight(1080U);
                                // windowWidth = game.GetWindowWidth();
                                // windowHeight = game.GetWindowHeight();
                                // window.setSize(sf::Vector2u(windowWidth, windowHeight));
                                break;
                            case BtnFunc::Back:
                                if (game.GetPlaying())
                                {
                                    if (game.GetMenuState() == MenuState::Inventory)
                                    {
                                        game.SetMenuState(MenuState::Playing);
                                    }
                                    else
                                    {
                                        game.SetMenuState(MenuState::Pause);
                                    }
                                }
                                else
                                {
                                    game.SetMenuState(MenuState::Main);
                                }
                                breakLoop = true;
                                break;
                            default:
                                break;
                            }
                            break;
                        }
                    }
                }
                else if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                {
                    // get the current mouse position in the window
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    // convert it to world coordinates
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    auto moveX = worldPos.x - viewCenter.x;
                    auto moveY = worldPos.y - viewCenter.y;

                    // RIGHT
                    if (moveX > 0U)
                    {
                        if (moveX + viewCenter.x >= gameWidth - (viewSize.x / 2U))
                        {
                            view->setCenter({gameWidth - (viewSize.x / 2U), viewCenter.y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }
                    else // LEFT
                    {
                        if (viewCenter.x + (moveX) <= 0U + (viewSize.x / 2U))
                        {
                            view->setCenter({viewSize.x / 2U, viewCenter.y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }
                    viewCenter = view->getCenter();

                    // UP
                    if (moveY < 0U)
                    {
                        if (moveY + viewCenter.y <= 0U + (viewSize.y / 2U))
                        {
                            view->setCenter({viewCenter.x, viewSize.y / 2U});
                        }
                        else
                        {
                            view->move({0U, moveY});
                        }
                    }
                    else // DOWN
                    {
                        if (moveY + viewCenter.y >= gameHeight - (viewSize.y / 2U))
                        {
                            view->setCenter({viewCenter.x, gameHeight - (viewSize.y / 2U)});
                        }
                        else
                        {
                            view->move({0U, moveY});
                        }
                    }
                    viewCenter = view->getCenter();
                    viewSize = view->getSize();
                }
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

                    default:
                        break;
                    }
                }
                break;
            case sf::Event::KeyReleased:
                if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                {
                    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)))
                    {
                        player.SetMovement(PlayerMove::NotMoving);
                    }
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
                {
                    if (event.mouseWheelScroll.delta > 0U)
                    {
                        if (game.GetZoom() < game.GetMaxZoom())
                        {
                            view->zoom(0.5F);
                            game.SetZoom(1U);
                        }
                    }
                    else
                    {
                        if (game.GetZoom() > 0U)
                        {
                            view->zoom(2.0F);
                            game.SetZoom(-1);
                        }
                    }

                    UpdateView(game, *view);
                    viewCenter = view->getCenter();
                    viewSize = view->getSize();
                }
                break;
            default:
                break;
            }

            if (breakLoop)
            {
                break;
            }
        }

        auto menuState = game.GetMenuState();
        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetPlaying() && menuState == MenuState::Playing)
        {
            window.setView(*view);

            DrawSurface(window, surfaces, player, playerSprite, game);

            HandlePlayerMovement(player, clock, playerSprite, game, world);
            window.draw(playerSprite);

            DrawWorld(window, world);


            for (const auto &data : allItems)
            {
                /*
                Todo: Implement Inventory, Draw Items on Ground, Add collect Item, Remove This Later
                */
                std::cout << "ItemID: " << data->GetID() << std::endl;
                std::cout << "ItemName: " << data->GetName() << std::endl;
            }
        }

        switch (menuState)
        {
        case MenuState::Pause:
            DrawMenu(window, menuView, titles, buttons, MenuState::Pause);
            break;
        case MenuState::Options:
            DrawMenu(window, menuView, titles, buttons, MenuState::Options);
            break;
        case MenuState::Main:
            DrawMenu(window, menuView, titles, buttons, MenuState::Main);
            break;
        case MenuState::Inventory:
            DrawMenu(window, menuView, titles, buttons, MenuState::Inventory);
            break;

        default:
            break;
        }

        window.display();
    }

    return 0;
}
