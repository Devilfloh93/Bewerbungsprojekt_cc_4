#pragma once
#include <cstdint>

enum class GameState
{
    MainMenu = 0,
    Paused,
    Running
};

struct PlayingView
{
    float centerX;
    float centerY;
    float sizeX;
    float sizeY;
};

class Game
{

public:
    Game(const PlayingView view);
    ~Game() = default;

    GameState GetGameState() const;
    std::uint8_t GetMaxZoom() const;
    std::uint8_t GetZoom() const;
    bool GetResize() const;
    PlayingView GetView() const;

    void SetGameState(const GameState gameState);
    void SetZoom(const std::uint8_t zoom);
    void SetResize(const bool resized);
    void SetView(const PlayingView view);

private:
    GameState m_gameState = GameState::MainMenu;
    std::uint8_t m_maxZoom = 3U;
    std::uint8_t m_zoom = 0U;
    bool m_resize = false;
    PlayingView m_view;
};
