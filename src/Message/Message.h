#pragma once
#include "Text.h"

using namespace std;

enum class MessageType
{
    Information,
    Error,
    Success
};

class Message final: public Text
{
public:
    Message(unique_ptr<sf::Text> text, unique_ptr<sf::Clock> clock);
    ~Message() = default;

    sf::Clock *GetClock() const;

private:
    unique_ptr<sf::Clock> m_clock;
};
