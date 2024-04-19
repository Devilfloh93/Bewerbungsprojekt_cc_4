#pragma once
#include "Game.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class Player;
class Game;

class Thread
{
public:
    Thread(const sf::RenderWindow &window, Game *game);
    ~Thread() = default;

    void Join();

    void UpdateStats(const sf::RenderWindow &window, Game *game);
    void SaveGame(const sf::RenderWindow &window, Game *game);

    // Function to returns false when
    // the thread is stopped
    template <class Duration>
    bool WaitFor(Duration duration);

private:
    vector<thread> m_threads;
    condition_variable m_conditionVar;
    mutex m_mutex;
    bool m_stop = false;
};
