#pragma once
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


class TextTrader : public Text
{
public:
    TextTrader(unique_ptr<sf::Text> text);
    ~TextTrader() = default;

    void SetSelectedItemID(const uint8_t ID);
    uint8_t GetSelectedItemID() const;

protected:
    uint8_t m_selectedItemID;
};
