#include "Game.h"

Game::Game()
{
}

GameState Game::GetGameState() const
{
    return this->m_gameState;
}

void Game::SetGameState(const GameState gameState)
{
    this->m_gameState = gameState;
}
