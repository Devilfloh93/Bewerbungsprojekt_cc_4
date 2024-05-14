#pragma once
#include "Gui.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Text
{
public:
    Text(unique_ptr<sf::Text> text);
    ~Text() = default;

    sf::Text *GetText() const;

protected:
    unique_ptr<sf::Text> m_text;
};


class SelectableText : public Text
{
public:
    SelectableText(unique_ptr<sf::Text> text,
                   const uint16_t ID,
                   const uint8_t selectedID,
                   const SelectedTextCategorie selectedCategorie);
    ~SelectableText() = default;

    uint16_t GetID() const;
    uint8_t GetSelectedID() const;
    SelectedTextCategorie GetSelectedCategorie() const;

protected:
    uint16_t m_ID;
    uint8_t m_selectedID;
    SelectedTextCategorie m_selectedCategorie;
};
