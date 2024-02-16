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

    //button01
    sf::Sprite button01;

    button01.setTexture(buttonTexture01);
    button01.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    button01.setScale(sf::Vector2f(5.0F, 5.0F));
    button01.setPosition(
        sf::Vector2f((gameWidth / 2) - ((button01.getLocalBounds().getSize().x * button01.getScale().x) / 2), 0.0F));

    sf::Text buttonText01;
    buttonText01.setFont(font);
    buttonText01.setCharacterSize(20U);


    //Button02
    sf::Sprite button02;
    sf::Text buttonText02;
    button02.setTexture(buttonTexture01);
    button02.setTextureRect(sf::IntRect(191U, 146U, 34U, 15U));
    button02.setScale(sf::Vector2f(5.0F, 5.0F));
    button02.setPosition(
        sf::Vector2f((gameWidth / 2) - ((button02.getLocalBounds().getSize().x * button02.getScale().x) / 2),
                     button01.getGlobalBounds().getSize().y));

    buttonText02.setFont(font);
    buttonText02.setCharacterSize(20U);


    // Tile Texture and Sprite
    sf::Sprite tileSprite;
    sf::Texture grassTexture01;
    grassTexture01.loadFromFile("ressources/textures/TileSet_V2.png");

    // Player Texture and Sprite
    sf::Sprite playerSprite;
    sf::Texture playerTexture01;
    playerTexture01.loadFromFile("ressources/textures/Human-Worker-Red.png");

    playerSprite.setTexture(playerTexture01);
    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
    playerSprite.setPosition(80.0F, 80.0F);

    for (size_t i = 0; i < gameAreaMaxTiles; ++i)
    {
        if (j * gameAreaTileSize >= gameAreaWidth)
        {
            j = 0;
            ++k;
        }

        tileSprite.setTexture(grassTexture01);

        if (k == 0 || j == 0 || j * gameAreaTileSize == (gameAreaWidth - gameAreaTileSize) ||
            k * gameAreaTileSize == (gameAreaHeight - gameAreaTileSize))
        {
            tileSprite.setTextureRect(sf::IntRect(0U, 416U, gameAreaTileSize, gameAreaTileSize));
        }
        else
        {
            switch (distGrassTexture(gen))
            {
            case 0:
                tileSprite.setTextureRect(sf::IntRect(0U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 1:
                tileSprite.setTextureRect(sf::IntRect(30U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 2:
                tileSprite.setTextureRect(sf::IntRect(63U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;
            case 3:
                tileSprite.setTextureRect(sf::IntRect(96U, 192U, gameAreaTileSize, gameAreaTileSize));
                break;

            default:
                break;
            }
        }

        tileSprite.setPosition(j * gameAreaTileSize, k * gameAreaTileSize);

        tileSprites.push_back(tileSprite);
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

            // TODO ADD MAIN MENU WITH START BUTTON AND REMOVE || game.GetGameState() == GameState::MainMenu

            if (game.GetGameState() == GameState::Paused || game.GetGameState() == GameState::MainMenu)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape)
                {
                    game.SetGameState(GameState::Running);
                    break;
                }
            }

            if (game.GetGameState() == GameState::Running)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape)
                {
                    game.SetGameState(GameState::Paused);
                    break;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::A)
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 200U, 16U, 16U));
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::D)
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 72U, 16U, 16U));
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::W)
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 136U, 16U, 16U));
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::S)
                {
                    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
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
                        if (moveX + view->getCenter().x >= gameAreaWidth - (gameWidth / 2U))
                        {
                            view->setCenter({gameAreaWidth - (gameWidth / 2U), view->getCenter().y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }
                    else // LEFT
                    {
                        if (view->getCenter().x + (moveX) <= 0U + (gameWidth / 2U))
                        {
                            view->setCenter({gameWidth / 2U, view->getCenter().y});
                        }
                        else
                        {
                            view->move({moveX, 0U});
                        }
                    }

                    // UP
                    if (moveY < 0U)
                    {
                        if (moveY + view->getCenter().y <= 0U + (gameHeight / 2U))
                        {
                            view->setCenter({view->getCenter().x, gameHeight / 2U});
                        }
                        else
                        {
                            view->move({0U, moveY});
                        }
                    }
                    else // DOWN
                    {
                        if (moveY + view->getCenter().y >= gameAreaHeight - (gameHeight / 2U))
                        {
                            view->setCenter({view->getCenter().x, gameAreaHeight - (gameHeight / 2U)});
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

            window.draw(playerSprite);
        }

        if (game.GetGameState() == GameState::Paused)
        {
            buttonText01.setString("Paused");

            window.draw(button01);
            window.draw(buttonText01);
        }

        if (game.GetGameState() == GameState::MainMenu)
        {
            buttonText01.setString("Main Menu");

            window.draw(button01);
            window.draw(buttonText01);

            buttonText02.setString("Play");

            window.draw(button02);
            window.draw(buttonText02);
        }

        buttonText01.setPosition(sf::Vector2f(
            button01.getGlobalBounds().getPosition().x +
                ((button01.getGlobalBounds().getSize().x / 2) - (buttonText01.getLocalBounds().getSize().x / 2)),
            button01.getGlobalBounds().getPosition().y +
                ((button01.getGlobalBounds().getSize().y / 2) - (buttonText01.getLocalBounds().getSize().y / 2))));

        buttonText02.setPosition(sf::Vector2f(
            button02.getGlobalBounds().getPosition().x +
                ((button02.getGlobalBounds().getSize().x / 2) - (buttonText02.getLocalBounds().getSize().x / 2)),
            button02.getGlobalBounds().getPosition().y +
                ((button02.getGlobalBounds().getSize().y / 2) - (buttonText02.getLocalBounds().getSize().y / 2))));

        window.display();
    }

    return 0;
}
