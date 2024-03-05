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
    GameState m_gameState;
    std::uint8_t m_maxZoom;
    std::uint8_t m_zoom;
    bool m_resize;
    PlayingView m_view;
};
