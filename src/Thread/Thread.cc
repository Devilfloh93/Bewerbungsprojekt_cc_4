#include "Thread.h"


Thread::Thread(const sf::RenderWindow &window, Player *player, Game *game)
{

    m_threads.push_back(thread(&Thread::CollisionCheck, this, ref(window), player, game));
    m_threads.push_back(thread(&Thread::UpdateStats, this, ref(window), player, game));
    m_threads.push_back(thread(&Thread::SaveGame, this, ref(window), game));
}

void Thread::Join()
{
    {
        lock_guard<mutex> l(m_mutex);
        m_stop = true;
    }
    m_conditionVar.notify_one();

    for (auto &data : m_threads)
    {
        if (data.joinable())
            data.join();
    }
}

template <class Duration>
bool Thread::WaitFor(Duration duration)
{
    unique_lock<mutex> l(m_mutex);
    return !m_conditionVar.wait_for(l, duration, [this]() { return m_stop; });
}

void Thread::CollisionCheck(const sf::RenderWindow &window, Player *player, Game *game)
{
    while (window.isOpen())
    {
        if (game->GetPlaying() && game->GetMenuState() == MenuState::Playing)
            player->CheckCollision(game);
    }
}

void Thread::UpdateStats(const sf::RenderWindow &window, Player *player, Game *game)
{
    while (window.isOpen())
    {
        this_thread::sleep_for(chrono::seconds(1));
        player->UpdateStats(game);
    }
}

void Thread::SaveGame(const sf::RenderWindow &window, Game *game)
{
    while (window.isOpen())
    {
        while (WaitFor(chrono::minutes(1)))
            game->GetPlayer()->Save();
    }
}
