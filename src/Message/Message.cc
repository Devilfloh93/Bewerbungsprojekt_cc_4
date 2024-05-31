#include "Message.h"

Message::Message(unique_ptr<sf::Text> text, unique_ptr<sf::Clock> clock, const uint16_t messageID)
    : Text(move(text)), m_clock(move(clock)), m_messageID(messageID)
{
}

sf::Clock *Message::GetClock() const
{
    return m_clock.get();
}

uint16_t Message::GetMessageID() const
{
    return m_messageID;
}
