#include "Thread.h"


Thread::Thread(Game *game) : m_stop(false)
{
    m_threads.push_back(thread(&Thread::UpdateStats, this, game));
    m_threads.push_back(thread(&Thread::SaveGame, this, game));
    m_threads.push_back(thread(&Thread::MoveCreature, this, game));
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
        {
            data.join();
        }
    }
    m_threads.clear();
}

template <class Duration>
bool Thread::WaitFor(Duration duration)
{
    unique_lock<mutex> l(m_mutex);
    return !m_conditionVar.wait_for(l, duration, [this]() { return m_stop; });
}

void Thread::UpdateStats(Game *game)
{
    auto player = game->GetPlayer();

    while (game->GetWindow()->isOpen() && game->GetPlaying())
    {
        this_thread::sleep_for(chrono::seconds(1));
        player->UpdateStats(game);
    }
}

void Thread::SaveGame(Game *game)
{
    while (game->GetWindow()->isOpen() && game->GetPlaying())
    {
        while (WaitFor(chrono::minutes(1)))
        {
            game->SetMenuState(MenuState::Saving, false);
            game->Saving(false);
            game->SetMenuState();
        }
    }
}

void Thread::MoveCreature(Game *game)
{
    while (game->GetWindow()->isOpen() && game->GetPlaying())
    {
        this_thread::sleep_for(chrono::seconds(1));
        game->MoveCreature();
    }
}
