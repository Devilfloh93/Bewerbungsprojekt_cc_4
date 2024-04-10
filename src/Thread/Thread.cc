#include "Thread.h"


Thread::Thread(const sf::RenderWindow &window, Player *player, Game &game)
{
    m_threads.push_back(thread(&Thread::CollisionCheck, this, ref(window), player, ref(game)));
    m_threads.push_back(thread(&Thread::UpdateStats, this, ref(window), player, ref(game)));
}

void Thread::Join()
{
    for (auto &data : m_threads)
    {
        data.join();
    }
}

void Thread::CollisionCheck(const sf::RenderWindow &window, Player *player, Game &game)
{
    while (window.isOpen())
    {
        if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
        {
            player->CheckCollision(game);
        }
    }
}

void Thread::UpdateStats(const sf::RenderWindow &window, Player *player, const Game &game)
{
    player->UpdateStats(window, game);
}
