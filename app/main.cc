#include "Game.h"
#include "Gui.h"
#include "Player.h"
#include "Surface.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

int main()
{
    const std::uint32_t gameAreaWidth = 8800U;
    const std::uint32_t gameAreaHeight = 4800U;
    const std::uint32_t surfaceTileSize = 32U;
    const std::uint32_t gameAreaMaxTiles = ((gameAreaWidth * gameAreaHeight) / surfaceTileSize) / surfaceTileSize;
    const auto defaultCenter = sf::Vector2f(gameWidth / 2, gameHeight / 2);

    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Good Game",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);
    auto view = std::make_unique<sf::View>(defaultCenter, sf::Vector2f(gameWidth, gameHeight));

    sf::Clock clock;

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    std::uniform_int_distribution<> distGrassTexture(0, 2);

    // Init Game
    auto game = Game(PlayingView(view->getCenter().x, view->getCenter().y, view->getSize().x, view->getSize().y));


    //Init Player
    auto player = Player("PlayerName", 100.0F, 100.0F, 100.0F, 1.0F);

    // vector with tilesprites
    std::vector<std::unique_ptr<Surface>> surfaces;

    size_t j = 0;
    size_t k = 0;

    sf::Sprite tileSprite;

    // Font
    sf::Font font;
    font.loadFromFile("ressources/font/Round9x13.ttf");

    //Button Texture
    sf::Texture btnTexture01;
    btnTexture01.loadFromFile("ressources/textures/Buttons_all_01.png");

    sf::Text mainTitleText;
    mainTitleText.setFont(font);
    mainTitleText.setCharacterSize(40U);

    //btn01
    sf::Sprite btn01;

    btn01.setTexture(btnTexture01);
    btn01.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    btn01.setScale(sf::Vector2f(5.0F, 5.0F));

    sf::Text btnText01;
    btnText01.setFont(font);
    btnText01.setCharacterSize(20U);

    //btn02
    sf::Sprite btn02;
    sf::Text btnText02;
    btn02.setTexture(btnTexture01);
    btn02.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    btn02.setScale(sf::Vector2f(5.0F, 5.0F));

    btnText02.setFont(font);
    btnText02.setCharacterSize(20U);

    sf::Texture surfaceTexture01;
    surfaceTexture01.loadFromFile("ressources/textures/TileSet_V2.png");

    // Player Texture and Sprite
    sf::Sprite playerSprite;
    sf::Texture playerTexture01;
    playerTexture01.loadFromFile("ressources/textures/Human-Worker-Red.png");

    playerSprite.setTexture(playerTexture01);
    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
    playerSprite.setPosition(80.0F, 80.0F);

    for (size_t i = 0; i < gameAreaMaxTiles; ++i)
    {
        auto surfaceType = SurfaceType::Grass;
        if (j * surfaceTileSize >= gameAreaWidth)
        {
            j = 0;
            ++k;
        }

        tileSprite.setTexture(surfaceTexture01);

        if (j == 0 && k == 0 || (j * surfaceTileSize == (gameAreaWidth - surfaceTileSize) && k == 0) ||
            (k * surfaceTileSize == (gameAreaHeight - surfaceTileSize) && j == 0) ||
            (j * surfaceTileSize == (gameAreaWidth - surfaceTileSize) &&
             k * surfaceTileSize == (gameAreaHeight - surfaceTileSize)))
        {
            // FULL WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 416U, surfaceTileSize, surfaceTileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (j == 0)
        {
            // LEFT WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 320U, surfaceTileSize, surfaceTileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (j * surfaceTileSize == (gameAreaWidth - surfaceTileSize))
        {
            // RIGHT WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 288U, surfaceTileSize, surfaceTileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (k == 0)
        {
            // TOP WATER
            tileSprite.setTextureRect(sf::IntRect(183U, 320U, surfaceTileSize, surfaceTileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (k * surfaceTileSize == (gameAreaHeight - surfaceTileSize))
        {
            // BOT WATER
            tileSprite.setTextureRect(sf::IntRect(128U, 288U, surfaceTileSize, surfaceTileSize));
            surfaceType = SurfaceType::Water;
        }
        // else if (k == 0 || j == 0 || k * surfaceTileSize == (gameAreaHeight - surfaceTileSize))
        // {
        //     // FULL WATER
        //     tileSprite.setTextureRect(sf::IntRect(0U, 416U, surfaceTileSize, surfaceTileSize));
        // }
        else
        {
            switch (distGrassTexture(gen))
            {
            case 0:
                tileSprite.setTextureRect(sf::IntRect(0U, 192U, surfaceTileSize, surfaceTileSize));
                break;
            case 1:
                tileSprite.setTextureRect(sf::IntRect(30U, 192U, surfaceTileSize, surfaceTileSize));
                break;
            case 2:
                tileSprite.setTextureRect(sf::IntRect(63U, 192U, surfaceTileSize, surfaceTileSize));
                break;
            case 3:
                tileSprite.setTextureRect(sf::IntRect(96U, 192U, surfaceTileSize, surfaceTileSize));
                break;

            default:
                break;
            }
            surfaceType = SurfaceType::Grass;
        }

        tileSprite.setPosition(j * surfaceTileSize, k * surfaceTileSize);

        switch (surfaceType)
        {
        case SurfaceType::Grass:
            surfaces.push_back(std::make_unique<Grass>(tileSprite, 1.5F));
            break;
        case SurfaceType::Water:
            surfaces.push_back(std::make_unique<Water>(tileSprite, 0.8F));
            break;

        default:
            break;
        }

        ++j;
    }


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

                    auto btnPos = btn01.getPosition();
                    auto btnLSize = btn01.getLocalBounds().getSize();
                    auto btnScale = btn01.getScale();

                    if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) &&
                        worldPos.y > btnPos.y && worldPos.y < btnPos.y + (btnLSize.y * btnScale.y))
                    {
                        if (game.GetGameState() == GameState::Paused)
                        {
                            game.SetResize(true);
                        }
                        game.SetGameState(GameState::Running);
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

                    if (view->getCenter().x >= gameAreaWidth - (view->getSize().x / 2U))
                    {
                        view->setCenter({gameAreaWidth - (view->getSize().x / 2U), view->getCenter().y});
                    }
                    if (view->getCenter().x <= 0U + (view->getSize().x / 2U))
                    {
                        view->setCenter({view->getSize().x / 2U, view->getCenter().y});
                    }
                    if (view->getCenter().y <= 0U + (view->getSize().y / 2U))
                    {
                        view->setCenter({view->getCenter().x, view->getSize().y / 2U});
                    }
                    if (view->getCenter().y >= gameAreaHeight - (view->getSize().y / 2U))
                    {
                        view->setCenter({view->getCenter().x, gameAreaHeight - (view->getSize().y / 2U)});
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
                        if (moveX + view->getCenter().x >= gameAreaWidth - (view->getSize().x / 2U))
                        {
                            view->setCenter({gameAreaWidth - (view->getSize().x / 2U), view->getCenter().y});
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
                        if (moveY + view->getCenter().y >= gameAreaHeight - (view->getSize().y / 2U))
                        {
                            view->setCenter({view->getCenter().x, gameAreaHeight - (view->getSize().y / 2U)});
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

                if (playerSprite.getPosition().x + playerSpeed < gameAreaWidth - surfaceTileSize)
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

                if (playerSprite.getPosition().y + playerSpeed < gameAreaHeight - surfaceTileSize)
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

                view->setSize(sf::Vector2f(gameWidth, gameHeight));
                view->setCenter(defaultCenter);

                game.SetResize(false);
            }
            window.setView(*view);

            mainTitleText.setString("Paused");
            window.draw(mainTitleText);

            btnText01.setString("Resume");

            window.draw(btn01);
            window.draw(btnText01);

            btnText02.setString("Not Used");

            window.draw(btn02);
            window.draw(btnText02);
        }

        if (game.GetGameState() == GameState::MainMenu)
        {
            mainTitleText.setString("Main Menu");
            window.draw(mainTitleText);

            btnText01.setString("Play");

            window.draw(btn01);
            window.draw(btnText01);

            btnText02.setString("Not Used");

            window.draw(btn02);
            window.draw(btnText02);
        }

        SetTitlePos(mainTitleText);
        SetBtnAndTextPos(btn01, mainTitleText, btnText01);
        SetBtnAndTextPos(btn02, btn01, btnText02);

        window.display();
    }

    return 0;
}
