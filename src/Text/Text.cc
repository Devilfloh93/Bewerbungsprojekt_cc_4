#include "Text.h"

Text::Text(sf::Text *text) : m_text(text)
{
}

sf::Text *Text::GetText() const
{
    return m_text;
}
