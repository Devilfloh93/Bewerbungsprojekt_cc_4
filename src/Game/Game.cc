#include "Game.h"

Game::Game()
{
    m_gameState = GameState::MainMenu;
    m_maxZoom = 3U;
    m_zoom = 0U;
}

GameState Game::GetGameState() const
{
    return this->m_gameState;
}

void Game::SetGameState(const GameState gameState)
{
    this->m_gameState = gameState;
}

std::uint8_t Game::GetMaxZoom() const
{
    return this->m_maxZoom;
}

std::uint8_t Game::GetZoom() const
{
    return this->m_zoom;
}

void Game::SetZoom(const std::uint8_t zoom)
{
    this->m_zoom += zoom;
}
