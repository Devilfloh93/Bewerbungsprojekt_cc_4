#include "Text.h"

Text::Text(unique_ptr<sf::Text> text) : m_text(move(text))
{
}

sf::Text *Text::GetText() const
{
    return m_text.get();
}

TextTrader::TextTrader(unique_ptr<sf::Text> text) : Text(move(text)), m_selectedItemID(0)
{
}

void TextTrader::SetSelectedItemID(const uint8_t ID)
{
    m_selectedItemID = ID;
}

uint8_t TextTrader::GetSelectedItemID() const
{
    return m_selectedItemID;
}
