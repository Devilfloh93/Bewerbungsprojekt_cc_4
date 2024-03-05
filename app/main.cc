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
    const std::uint16_t windowWidth = 1280U;
    const std::uint16_t windowHeight = 720U;
    const std::uint32_t gameWidth = 8800U;
    const std::uint32_t gameHeight = 4800U;
    const std::uint32_t surfaceTileSize = 32U;
    const std::uint32_t gameMaxTiles = ((gameWidth * gameHeight) / surfaceTileSize) / surfaceTileSize;
    const auto defaultCenter = sf::Vector2f(windowWidth / 2U, windowHeight / 2U);

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
    auto game = Game(PlayingView(view->getCenter().x, view->getCenter().y, view->getSize().x, view->getSize().y));

    //Player Init
    auto player = Player("PlayerName", 100.0F, 100.0F, 100.0F, 1.0F);
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
        ButtonCfg{.textCfg = TextCfg{.guiCfg = GuiCfg{.gameState = GameState::MainMenu},
                                     .text = "Not Used Main Menu",
                                     .fontSize = 20U},
                  .btnFnc = BtnFunc::Nothing,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}},
        ButtonCfg{.textCfg =
                      TextCfg{.guiCfg = GuiCfg{.gameState = GameState::Paused}, .text = "Resume", .fontSize = 20U},
                  .btnFnc = BtnFunc::Play,
                  .textureRect = {191U, 146U, 34U, 15U},
                  .scale = {5.0F, 5.0F}},
        ButtonCfg{
            .textCfg =
                TextCfg{.guiCfg = GuiCfg{.gameState = GameState::Paused}, .text = "Not Used Paused", .fontSize = 20U},
            .btnFnc = BtnFunc::Nothing,
            .textureRect = {191U, 146U, 34U, 15U},
            .scale = {5.0F, 5.0F}}};

    InitMenus(windowWidth, font01, btnTexture01, menus, menuTitles, menuButtons);
    InitPlayer(playerSprite, playerTexture01);
    InitSurface(surfaces, surfaceTileSize, gameMaxTiles, gameWidth, gameWidth, surfaceTexture01);

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
                            worldPos.y > btnPos.y && worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) &&
                            data->GetBtnFnc() == BtnFunc::Play)
                        {
                            if (game.GetGameState() == GameState::Paused)
                            {
                                game.SetResize(true);
                            }
                            game.SetGameState(GameState::Running);
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
                        game.SetResize(true);
                        game.SetGameState(GameState::Paused);
                        break;
                    }

                    switch (event.key.code)
                    {
                    case sf::Keyboard::Key::A:
                        playerSprite.setTextureRect(sf::IntRect(8U, 200U, 16U, 16U));
                        player.SetMovement(PlayerMove::Left);
                        break;
                    case sf::Keyboard::Key::D:
                        playerSprite.setTextureRect(sf::IntRect(8U, 72U, 16U, 16U));
                        player.SetMovement(PlayerMove::Right);
                        break;
                    case sf::Keyboard::Key::W:
                        playerSprite.setTextureRect(sf::IntRect(8U, 136U, 16U, 16U));
                        player.SetMovement(PlayerMove::Up);
                        break;
                    case sf::Keyboard::Key::S:
                        playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
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

                    if (view->getCenter().x >= gameWidth - (view->getSize().x / 2U))
                    {
                        view->setCenter({gameWidth - (view->getSize().x / 2U), view->getCenter().y});
                    }
                    if (view->getCenter().x <= 0U + (view->getSize().x / 2U))
                    {
                        view->setCenter({view->getSize().x / 2U, view->getCenter().y});
                    }
                    if (view->getCenter().y <= 0U + (view->getSize().y / 2U))
                    {
                        view->setCenter({view->getCenter().x, view->getSize().y / 2U});
                    }
                    if (view->getCenter().y >= gameHeight - (view->getSize().y / 2U))
                    {
                        view->setCenter({view->getCenter().x, gameHeight - (view->getSize().y / 2U)});
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    // get the current mouse position in the window
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    // convert it to world coordinates
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    auto moveX = worldPos.x - view->getCenter().x;
                    auto moveY = worldPos.y - view->getCenter().y;

                    // RIGHT
                    if (moveX > 0U)
                    {
                        if (moveX + view->getCenter().x >= gameWidth - (view->getSize().x / 2U))
                        {
                            view->setCenter({gameWidth - (view->getSize().x / 2U), view->getCenter().y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }
                    else // LEFT
                    {
                        if (view->getCenter().x + (moveX) <= 0U + (view->getSize().x / 2U))
                        {
                            view->setCenter({view->getSize().x / 2U, view->getCenter().y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }

                    // UP
                    if (moveY < 0U)
                    {
                        if (moveY + view->getCenter().y <= 0U + (view->getSize().y / 2U))
                        {
                            view->setCenter({view->getCenter().x, view->getSize().y / 2U});
                        }
                        else
                        {
                            view->move({0U, moveY});
                        }
                    }
                    else // DOWN
                    {
                        if (moveY + view->getCenter().y >= gameHeight - (view->getSize().y / 2U))
                        {
                            view->setCenter({view->getCenter().x, gameHeight - (view->getSize().y / 2U)});
                        }
                        else
                        {
                            view->move({0U, moveY});
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(50U, 50U, 50U));

        if (game.GetGameState() == GameState::Running)
        {
            auto playerPos = playerSprite.getPosition();
            auto playerSpeed = player.GetSpeed();

            if (game.GetResize())
            {
                view->setSize(sf::Vector2f(game.GetView().sizeX, game.GetView().sizeY));
                view->setCenter(sf::Vector2f(game.GetView().centerX, game.GetView().centerY));
                game.SetResize(false);
            }
            window.setView(*view);

            // Draw it
            for (auto &data : surfaces)
            {
                auto sprite = data->GetSprite();
                auto posX = sprite.getPosition().x;
                auto posY = sprite.getPosition().y;
                auto tileSize = surfaceTileSize / 2;

                if (playerPos.x >= posX - tileSize && playerPos.x <= posX + tileSize &&
                    playerPos.y >= posY - tileSize && playerPos.y <= posY + tileSize)
                {
                    auto speed = data->GetSpeed();
                    player.SetSpeed(speed);
                }

                window.draw(sprite);
            }

            sf::Time elapsed = clock.getElapsedTime();

            switch (player.GetMovement())
            {
            case PlayerMove::Left:
                if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
                {
                    // RIGHT FEET
                    playerSprite.setTextureRect(sf::IntRect(72U, 200U, 16U, 16U));
                }
                else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
                {
                    // LEFT FEET
                    playerSprite.setTextureRect(sf::IntRect(8U, 200U, 16U, 16U));
                }
                else
                {
                    clock.restart();
                }

                if (playerSprite.getPosition().x - playerSpeed > 0 + (surfaceTileSize / 2))
                {
                    playerSprite.setPosition(playerPos.x - playerSpeed, playerPos.y);
                }
                else
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                }
                break;
            case PlayerMove::Right:
                if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
                {
                    // RIGHT FEET
                    playerSprite.setTextureRect(sf::IntRect(72U, 72U, 16U, 16U));
                }
                else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
                {
                    // LEFT FEET
                    playerSprite.setTextureRect(sf::IntRect(8U, 72U, 16U, 16U));
                }
                else
                {
                    clock.restart();
                }

                if (playerSprite.getPosition().x + playerSpeed < gameWidth - surfaceTileSize)
                {
                    playerSprite.setPosition(playerPos.x + playerSpeed, playerPos.y);
                }
                else
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                }
                break;
            case PlayerMove::Down:
                if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
                {
                    // RIGHT FEET
                    playerSprite.setTextureRect(sf::IntRect(72U, 8U, 16U, 16U));
                }
                else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
                {
                    // LEFT FEET
                    playerSprite.setTextureRect(sf::IntRect(136U, 8U, 16U, 16U));
                }
                else
                {
                    clock.restart();
                }

                if (playerSprite.getPosition().y + playerSpeed < gameHeight - surfaceTileSize)
                {
                    playerSprite.setPosition(playerPos.x, playerPos.y + playerSpeed);
                }
                else
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                }

                break;
            case PlayerMove::Up:
                if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
                {
                    // RIGHT FEET
                    playerSprite.setTextureRect(sf::IntRect(72U, 136U, 16U, 16U));
                }
                else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
                {
                    // LEFT FEET
                    playerSprite.setTextureRect(sf::IntRect(136U, 136U, 16U, 16U));
                }
                else
                {
                    clock.restart();
                }

                if (playerSprite.getPosition().y - playerSpeed > 0 + (surfaceTileSize / 2))
                {
                    playerSprite.setPosition(playerPos.x, playerPos.y - playerSpeed);
                }
                else
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                }

                break;
            case PlayerMove::NotMoving:
                playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                break;

            default:
                break;
            }

            window.draw(playerSprite);
        }

        if (game.GetGameState() == GameState::Paused)
        {
            if (game.GetResize())
            {
                game.SetView(
                    PlayingView(view->getCenter().x, view->getCenter().y, view->getSize().x, view->getSize().y));

                view->setSize(sf::Vector2f(windowWidth, windowHeight));
                view->setCenter(defaultCenter);

                game.SetResize(false);
            }
            window.setView(*view);

            for (const auto &data : menus)
            {
                if (data->GetGameState() == GameState::Paused)
                {
                    window.draw(data->GetSprite());
                    window.draw(data->GetText());
                }
            }
        }

        if (game.GetGameState() == GameState::MainMenu)
        {
            for (const auto &data : menus)
            {
                if (data->GetGameState() == GameState::MainMenu)
                {
                    window.draw(data->GetSprite());
                    window.draw(data->GetText());
                }
            }
        }

        window.display();
    }

    return 0;
}
