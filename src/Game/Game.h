#pragma once
#include <cstdint>

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
    GameState m_gameState = GameState::MainMenu;
    std::uint8_t m_maxZoom = 3U;
    std::uint8_t m_zoom = 1U;
};
