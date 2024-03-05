#include "Game.h"

Game::Game(const PlayingView view) : m_view(view)
{
    m_gameState = GameState::MainMenu;
    m_maxZoom = 3U;
    m_zoom = 0U;
    m_resize = false;
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

bool Game::GetResize() const
{
    return this->m_resize;
}

PlayingView Game::GetView() const
{
    return this->m_view;
}

void Game::SetZoom(const std::uint8_t zoom)
{
    this->m_zoom += zoom;
}

void Game::SetResize(const bool resize)
{
    this->m_resize = resize;
}

void Game::SetView(const PlayingView view)
{
    this->m_view = view;
}
