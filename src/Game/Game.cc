#include "Game.h"

Game::Game(const std::uint16_t windowWidth, const std::uint16_t windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    m_gameState = GameState::MainMenu;
    m_maxZoom = 3U;
    m_zoom = 0U;

    m_gameWidth = 8800U;
    m_gameHeight = 4800U;
    m_tileSize = 32U;
    m_maxTiles = ((m_gameWidth * m_gameHeight) / m_tileSize) / m_tileSize;
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

std::uint16_t Game::GetWindowWidth() const
{
    return this->m_windowWidth;
}

std::uint16_t Game::GetWindowHeight() const
{
    return this->m_windowHeight;
}

std::uint32_t Game::GetMaxTiles() const
{
    return this->m_maxTiles;
}

std::uint32_t Game::GetTileSize() const
{
    return this->m_tileSize;
}

std::uint32_t Game::GetGameWidth() const
{
    return this->m_gameWidth;
}
std::uint32_t Game::GetGameHeight() const
{
    return this->m_gameHeight;
}

void Game::SetWindowHeight(const std::uint32_t height)
{
    this->m_windowHeight = height;
}
void Game::SetWindowWidth(const std::uint32_t width)
{
    this->m_windowWidth = width;
}
