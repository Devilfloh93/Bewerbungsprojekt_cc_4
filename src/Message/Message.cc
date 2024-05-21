#include "Message.h"

Message::Message(unique_ptr<sf::Text> text, unique_ptr<sf::Clock> clock) : Text(move(text)), m_clock(move(clock))
{
}

sf::Clock *Message::GetClock() const
{
    return m_clock.get();
}
