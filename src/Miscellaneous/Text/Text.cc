#include "Text.h"

Text::Text(unique_ptr<sf::Text> text) : m_text(move(text))
{
}

sf::Text *Text::GetText() const
{
    return m_text.get();
}

SelectableText::SelectableText(unique_ptr<sf::Text> text, const uint16_t ID, const uint8_t selectedID)
    : Text(move(text)), m_ID(ID), m_selectedID(selectedID)
{
}

uint16_t SelectableText::GetID() const
{
    return m_ID;
}

uint8_t SelectableText::GetSelectedID() const
{
    return m_selectedID;
}
