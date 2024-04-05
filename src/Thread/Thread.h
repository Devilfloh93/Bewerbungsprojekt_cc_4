#pragma once
#include "Game.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

class Thread
{

public:
    Thread(const sf::RenderWindow &window, Player &player, Game &game);
    ~Thread() = default;

    void Join();

    void CollisionCheck(const sf::RenderWindow &window, Player &player, Game &game);
    void UpdateStats(const sf::RenderWindow &window, Player &player, const Game &game);

private:
    vector<thread> m_threads;
};
