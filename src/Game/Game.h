#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

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
    MainMenu = 0,
    Paused,
    Running
};

class Game
{

public:
    Game();
    ~Game() = default;

    GameState GetGameState() const;
    std::uint8_t GetMaxZoom() const;
    std::uint8_t GetZoom() const;

    void SetGameState(const GameState gameState);
    void SetZoom(const std::uint8_t zoom);

private:
    GameState m_gameState;
    std::uint8_t m_maxZoom;
    std::uint8_t m_zoom;
};
