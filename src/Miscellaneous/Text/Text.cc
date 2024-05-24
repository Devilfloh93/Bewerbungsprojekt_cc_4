#include "Text.h"

Text::Text(unique_ptr<sf::Text> text) : m_text(move(text))
{
}

sf::Text *Text::GetText() const
{
    return m_text.get();
}

SelectableText::SelectableText(unique_ptr<sf::Text> text,
                               const uint16_t selectedTextID,
                               const uint8_t selectedID,
                               const SelectedTextCategorie selectedCategorie)
    : Text(move(text)), m_selectedTextID(selectedTextID), m_selectedID(selectedID),
      m_selectedCategorie(selectedCategorie)
{
}

uint16_t SelectableText::GetSelectedTextID() const
{
    return m_selectedTextID;
}

uint8_t SelectableText::GetSelectedID() const
{
    return m_selectedID;
}

SelectedTextCategorie SelectableText::GetSelectedCategorie() const
{
    return m_selectedCategorie;
}
