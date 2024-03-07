#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

struct ReturnView
{
    sf::Vector2f viewCenter;
    sf::Vector2f viewSize;
};

struct MovementTexture
{
    sf::IntRect notMoving;
    sf::IntRect up01;
    sf::IntRect up02;
    sf::IntRect down01;
    sf::IntRect down02;
    sf::IntRect left01;
    sf::IntRect left02;
    sf::IntRect right01;
    sf::IntRect right02;
};

enum class GameState
{
    All = 0,
    MainMenu,
    Paused,
    Running,
    Options
};

class Game
{

public:
    Game(const std::uint16_t windowWidth, const std::uint16_t windowHeight);
    ~Game() = default;

    GameState GetGameState() const;
    std::uint8_t GetMaxZoom() const;
    std::uint8_t GetZoom() const;
    std::uint16_t GetWindowWidth() const;
    std::uint16_t GetWindowHeight() const;
    std::uint32_t GetMaxTiles() const;
    std::uint8_t GetTileSize() const;
    std::uint16_t GetGameWidth() const;
    std::uint16_t GetGameHeight() const;

    void SetGameState(const GameState gameState);
    void SetZoom(const std::uint8_t zoom);
    void SetWindowHeight(const std::uint16_t height);
    void SetWindowWidth(const std::uint16_t width);

private:
    GameState m_gameState;
    std::uint8_t m_maxZoom;
    std::uint8_t m_zoom;
    std::uint16_t m_windowWidth;
    std::uint16_t m_windowHeight;
    std::uint16_t m_gameWidth;
    std::uint16_t m_gameHeight;
    std::uint8_t m_tileSize;
    std::uint32_t m_maxTiles;
};

ReturnView InitView(Game &game, sf::View &view);

void UpdateView(const Game &game, sf::View &view);
