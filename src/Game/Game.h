#pragma once

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

    void SetGameState(const GameState gameState);

private:
    GameState m_gameState = GameState::MainMenu;
};
