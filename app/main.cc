#include "Game.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

int main()
{
    // GAME WINDOW
    const std::uint16_t gameWidth = 1280U;
    const std::uint16_t gameHeight = 720U;
    const std::uint32_t gameAreaWidth = 8800U;
    const std::uint32_t gameAreaHeight = 4800U;
    const std::uint32_t gameAreaTileSize = 32U;
    const std::uint32_t gameAreaMaxTiles = ((gameAreaWidth * gameAreaHeight) / gameAreaTileSize) / gameAreaTileSize;

    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "GoodGame",
                            sf::Style::Close); // Create window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60U);
    window.setKeyRepeatEnabled(false);

    auto view =
        std::make_unique<sf::View>(sf::Vector2f(gameWidth / 2, gameHeight / 2), sf::Vector2f(gameWidth, gameHeight));

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    std::uniform_int_distribution<> distGrassTexture(0, 2);

    // Init Game
    auto game = Game();

    //Init Player
    auto player = Player("Name", 100.0F, 100.0F, 100.0F);

    // vector with tilesprites
    std::vector<sf::Sprite> tileSprites;

    size_t j = 0;
    size_t k = 0;

    // Font
    sf::Font font;
    font.loadFromFile("ressources/font/Round9x13.ttf");

    //Button Texture
    sf::Texture buttonTexture01;
    buttonTexture01.loadFromFile("ressources/textures/Buttons_all_01.png");

    sf::Text mainTitleText;
    mainTitleText.setFont(font);
    mainTitleText.setCharacterSize(40U);

    //button01
    sf::Sprite button01;

    button01.setTexture(buttonTexture01);
    button01.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    button01.setScale(sf::Vector2f(5.0F, 5.0F));


    sf::Text buttonText01;
    buttonText01.setFont(font);
    buttonText01.setCharacterSize(20U);

    //Button02
    sf::Sprite button02;
    sf::Text buttonText02;
    button02.setTexture(buttonTexture01);
    button02.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    button02.setScale(sf::Vector2f(5.0F, 5.0F));


    buttonText02.setFont(font);
    buttonText02.setCharacterSize(20U);


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
        auto tileSprite = std::make_unique<sf::Sprite>();
        if (j * gameAreaTileSize >= gameAreaWidth)
        {
            j = 0;
            ++k;
        }

        tileSprite->setTexture(surfaceTexture01);

        if (j == 0 && k == 0 || (j * gameAreaTileSize == (gameAreaWidth - gameAreaTileSize) && k == 0) ||
            (k * gameAreaTileSize == (gameAreaHeight - gameAreaTileSize) && j == 0) ||
            (j * gameAreaTileSize == (gameAreaWidth - gameAreaTileSize) &&
             k * gameAreaTileSize == (gameAreaHeight - gameAreaTileSize)))
        {
            // FULL WATER
            tileSprite->setTextureRect(sf::IntRect(0U, 416U, gameAreaTileSize, gameAreaTileSize));
        }
        else if (j == 0)
        {
            // LEFT WATER
            tileSprite->setTextureRect(sf::IntRect(0U, 320U, gameAreaTileSize, gameAreaTileSize));
        }
        else if (j * gameAreaTileSize == (gameAreaWidth - gameAreaTileSize))
        {
            // RIGHT WATER
            tileSprite->setTextureRect(sf::IntRect(0U, 288U, gameAreaTileSize, gameAreaTileSize));
        }
        else if (k == 0)
        {
            // TOP WATER
            tileSprite->setTextureRect(sf::IntRect(183U, 320U, gameAreaTileSize, gameAreaTileSize));
        }
        else if (k * gameAreaTileSize == (gameAreaHeight - gameAreaTileSize))
        {
            // BOT WATER
            tileSprite->setTextureRect(sf::IntRect(128U, 288U, gameAreaTileSize, gameAreaTileSize));
        }
        // else if (k == 0 || j == 0 || k * gameAreaTileSize == (gameAreaHeight - gameAreaTileSize))
        // {
        //     // FULL WATER
        //     tileSprite.setTextureRect(sf::IntRect(0U, 416U, gameAreaTileSize, gameAreaTileSize));
        // }
        else
        {
            switch (distGrassTexture(gen))
            {
            case 0:
                tileSprite->setTextureRect(sf::IntRect(0U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 1:
                tileSprite->setTextureRect(sf::IntRect(30U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 2:
                tileSprite->setTextureRect(sf::IntRect(63U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 3:
                tileSprite->setTextureRect(sf::IntRect(96U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;

            default:
                break;
            }
        }

        tileSprite->setPosition(j * gameAreaTileSize, k * gameAreaTileSize);

        tileSprites.push_back(*tileSprite);
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
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    // convert it to world coordinates
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    if (worldPos.x > button01.getPosition().x &&
                        worldPos.x < button01.getPosition().x +
                                         (button01.getLocalBounds().getSize().x * button01.getScale().x) &&
                        worldPos.y > button01.getPosition().y &&
                        worldPos.y <
                            button01.getPosition().y + (button01.getLocalBounds().getSize().y * button01.getScale().y))
                    {
                        game.SetGameState(GameState::Running);
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
                        playerSprite.setTextureRect(sf::IntRect(8U, 200U, 16U, 16U));
                        player.SetMovement(PlayerMovement::Left);
                        break;
                    case sf::Keyboard::Key::D:
                        playerSprite.setTextureRect(sf::IntRect(8U, 72U, 16U, 16U));
                        player.SetMovement(PlayerMovement::Right);
                        break;
                    case sf::Keyboard::Key::W:
                        playerSprite.setTextureRect(sf::IntRect(8U, 136U, 16U, 16U));
                        player.SetMovement(PlayerMovement::Up);
                        break;
                    case sf::Keyboard::Key::S:
                        playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
                        player.SetMovement(PlayerMovement::Down);
                        break;

                    default:
                        break;
                    }
                }

                if (event.type == sf::Event::KeyReleased)
                {
                    player.SetMovement(PlayerMovement::NotMoving);
                }

                if (event.type == sf::Event::MouseWheelScrolled)
                {
                    if (event.mouseWheelScroll.delta > 0U)
                    {
                        if (game.GetZoom() <= game.GetMaxZoom())
                        {
                            view->zoom(0.5F);
                            game.SetZoom(game.GetZoom() + 1U);
                        }
                    }
                    else
                    {
                        if (game.GetZoom() > 1U)
                        {
                            view->zoom(2.0F);
                            game.SetZoom(game.GetZoom() - 1U);
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
            window.setView(*view);

            // Draw it
            for (auto &data : tileSprites)
            {
                window.draw(data);
            }

            auto playerPosX = playerSprite.getPosition().x;
            auto playerPosY = playerSprite.getPosition().y;

            switch (player.GetMovement())
            {
            case PlayerMovement::Left:
                playerSprite.setPosition(playerPosX - 1.0F, playerPosY);
                break;
            case PlayerMovement::Right:
                playerSprite.setPosition(playerPosX + 1.0F, playerPosY);
                break;
            case PlayerMovement::Down:
                playerSprite.setPosition(playerPosX, playerPosY + 1.0F);
                break;
            case PlayerMovement::Up:
                playerSprite.setPosition(playerPosX, playerPosY - 1.0F);
                break;

            default:
                break;
            }


            window.draw(playerSprite);
        }

        if (game.GetGameState() == GameState::Paused)
        {
            mainTitleText.setString("Paused");
            window.draw(mainTitleText);

            buttonText01.setString("Resume");

            window.draw(button01);
            window.draw(buttonText01);

            buttonText02.setString("Not Used");

            window.draw(button02);
            window.draw(buttonText02);
        }

        if (game.GetGameState() == GameState::MainMenu)
        {
            mainTitleText.setString("Main Menu");
            window.draw(mainTitleText);

            buttonText01.setString("Play");

            window.draw(button01);
            window.draw(buttonText01);

            buttonText02.setString("Not Used");

            window.draw(button02);
            window.draw(buttonText02);
        }

        mainTitleText.setPosition(
            sf::Vector2f((gameWidth / 2U) - (mainTitleText.getLocalBounds().getSize().x / 2U), 0.0F));

        button01.setPosition(sf::Vector2f(
            (gameWidth / 2U) - ((button01.getLocalBounds().getSize().x * button01.getScale().x) / 2U),
            (mainTitleText.getGlobalBounds().getPosition().y + mainTitleText.getLocalBounds().getSize().y) + 50.0F));

        buttonText01.setPosition(sf::Vector2f(
            button01.getGlobalBounds().getPosition().x +
                ((button01.getGlobalBounds().getSize().x / 2U) - (buttonText01.getLocalBounds().getSize().x / 2U)),
            button01.getGlobalBounds().getPosition().y +
                ((button01.getGlobalBounds().getSize().y / 2U) - (buttonText01.getLocalBounds().getSize().y / 2U))));

        button02.setPosition(
            sf::Vector2f((gameWidth / 2U) - ((button02.getLocalBounds().getSize().x * button02.getScale().x) / 2U),
                         (button01.getGlobalBounds().getPosition().y +
                          (button01.getLocalBounds().getSize().y * button01.getScale().y))));

        buttonText02.setPosition(sf::Vector2f(
            button02.getGlobalBounds().getPosition().x +
                ((button02.getGlobalBounds().getSize().x / 2U) - (buttonText02.getLocalBounds().getSize().x / 2U)),
            button02.getGlobalBounds().getPosition().y +
                ((button02.getGlobalBounds().getSize().y / 2U) - (buttonText02.getLocalBounds().getSize().y / 2U))));

        window.display();
    }

    return 0;
}
