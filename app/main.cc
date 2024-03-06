#include "Game.h"
#include "Gui.h"
#include "Player.h"
#include "Surface.h"
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
    auto game = Game(windowWidth, windowHeight);
    std::uint32_t gameWidth = game.GetGameWidth();
    std::uint32_t gameHeight = game.GetGameHeight();

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
    std::vector<std::unique_ptr<Surface>> surfaces;
    std::vector<std::unique_ptr<Text>> menus;

    // Fonts
    sf::Font font01;
    font01.loadFromFile("ressources/font/Round9x13.ttf");

    //Textures
    sf::Texture btnTexture01;
    btnTexture01.loadFromFile("ressources/textures/Buttons_all_01.png");

    sf::Texture surfaceTexture01;
    surfaceTexture01.loadFromFile("ressources/textures/TileSet_V2.png");

    sf::Texture playerTexture01;
    playerTexture01.loadFromFile("ressources/textures/Human-Worker-Red.png");

    // Init Game
    auto viewCenter = view->getCenter();
    auto viewSize = view->getSize();


    //Player Init
    auto player = Player("PlayerName", PlayerSurvivalStats{100.0F, 100.0F, 100.0F}, 1.0F);
    sf::Sprite playerSprite;

    // Menus
    const std::vector<TextCfg> menuTitles = {
        TextCfg{.guiCfg = GuiCfg{.gameState = GameState::MainMenu}, .text = "Main Menu", .fontSize = 40U},
        TextCfg{.guiCfg = GuiCfg{.gameState = GameState::Paused}, .text = "Paused", .fontSize = 40U}};

    const std::vector<ButtonCfg> menuButtons = {
        ButtonCfg{.textCfg =
                      TextCfg{.guiCfg = GuiCfg{.gameState = GameState::MainMenu}, .text = "Play", .fontSize = 20U},
                  .btnFnc = BtnFunc::Play,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}},
        ButtonCfg{.textCfg =
                      TextCfg{.guiCfg = GuiCfg{.gameState = GameState::Paused}, .text = "Resume", .fontSize = 20U},
                  .btnFnc = BtnFunc::Play,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}},
        ButtonCfg{.textCfg = TextCfg{.guiCfg = GuiCfg{.gameState = GameState::All}, .text = "Options", .fontSize = 20U},
                  .btnFnc = BtnFunc::Options,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}},
        ButtonCfg{.textCfg = TextCfg{.guiCfg = GuiCfg{.gameState = GameState::All}, .text = "Quit", .fontSize = 20U},
                  .btnFnc = BtnFunc::Quit,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}}};

    InitMenus(game, font01, btnTexture01, menus, menuTitles, menuButtons);
    InitPlayer(playerSprite, playerTexture01);
    InitSurface(surfaces, game, surfaceTexture01);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            if (game.GetGameState() == GameState::Paused || game.GetGameState() == GameState::MainMenu)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    // get the current mouse position in the window
                    auto pixelPos = sf::Mouse::getPosition(window);
                    // convert it to world coordinates
                    auto worldPos = window.mapPixelToCoords(pixelPos);
                    bool btnClicked = false;
                    for (const auto &data : menus)
                    {
                        auto btnPos = data->GetSprite().getPosition();
                        auto btnLSize = data->GetSprite().getLocalBounds().getSize();
                        auto btnScale = data->GetSprite().getScale();

                        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) &&
                            worldPos.y > btnPos.y && worldPos.y < btnPos.y + (btnLSize.y * btnScale.y))
                        {
                            switch (data->GetBtnFnc())
                            {
                            case BtnFunc::Play:
                                game.SetGameState(GameState::Running);
                                break;
                            case BtnFunc::Quit:
                                window.close();
                                break;
                            case BtnFunc::Options:
                                game.SetWindowWidth(1920U);
                                game.SetWindowHeight(1080U);
                                // windowWidth = 1920U;
                                // windowHeight = 1080U;
                                // defaultCenter = sf::Vector2f(windowWidth / 2U, windowHeight / 2U);
                                // menuView.setSize(sf::Vector2f(windowWidth, windowHeight));
                                // menuView.setCenter(defaultCenter);
                                // window.setSize(sf::Vector2u(windowWidth, windowHeight));
                                break;
                            default:
                                break;
                            }

                            btnClicked = true;
                            break;
                        }
                    }
                    if (btnClicked)
                    {
                        break;
                    }
                }
            }

            if (game.GetGameState() == GameState::Running)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Key::Escape)
                    {
                        game.SetGameState(GameState::Paused);
                        break;
                    }

                    switch (event.key.code)
                    {
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

                if (event.type == sf::Event::KeyReleased)
                {
                    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)))
                    {
                        player.SetMovement(PlayerMove::NotMoving);
                    }
                }

                if (event.type == sf::Event::MouseWheelScrolled)
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
                            game.SetZoom(-1U);
                        }
                    }
                    viewSize = view->getSize();

                    if (viewCenter.x >= gameWidth - (viewSize.x / 2U))
                    {
                        view->setCenter({gameWidth - (viewSize.x / 2U), viewCenter.y});
                        viewCenter = view->getCenter();
                    }
                    if (viewCenter.x <= 0U + (viewSize.x / 2U))
                    {
                        view->setCenter({viewSize.x / 2U, viewCenter.y});
                        viewCenter = view->getCenter();
                    }
                    if (viewCenter.y <= 0U + (viewSize.y / 2U))
                    {
                        view->setCenter({viewCenter.x, viewSize.y / 2U});
                        viewCenter = view->getCenter();
                    }
                    if (viewCenter.y >= gameHeight - (viewSize.y / 2U))
                    {
                        view->setCenter({viewCenter.x, gameHeight - (viewSize.y / 2U)});
                        viewCenter = view->getCenter();
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed)
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
                }
            }
        }

        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetGameState() == GameState::Running)
        {
            window.setView(*view);

            DrawSurface(window, surfaces, player, playerSprite, game);
            HandlePlayerMovement(player, clock, playerSprite, game);

            window.draw(playerSprite);
        }

        if (game.GetGameState() == GameState::Paused)
        {
            DrawMenu(window, menuView, menus, GameState::Paused);
        }

        if (game.GetGameState() == GameState::MainMenu)
        {
            DrawMenu(window, menuView, menus, GameState::MainMenu);
        }

        window.display();
    }

    return 0;
}
