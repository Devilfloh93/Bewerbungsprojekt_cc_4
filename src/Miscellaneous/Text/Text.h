#pragma once
#include "Gui.h"
#include <SFML/Graphics.hpp>

using namespace std;

struct ReturnTextAndSelectedID
{
    sf::Text *text;
    uint8_t id;
};


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
                   const uint16_t selectedTextID,
                   const uint8_t selectedID,
                   const SelectedTextCategorie selectedCategorie);
    ~SelectableText() = default;

    uint16_t GetSelectedTextID() const;
    uint8_t GetSelectedID() const;
    SelectedTextCategorie GetSelectedCategorie() const;

    bool GetDoubleClicked() const;
    void SetDoubleClicked(const bool doubleClicked);

protected:
    uint16_t m_selectedTextID;
    uint8_t m_selectedID;
    SelectedTextCategorie m_selectedCategorie;
    bool m_doubleClicked;
};
