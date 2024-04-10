#pragma once
#include <SFML/Graphics.hpp>

class Text
{
public:
    Text(sf::Text *text);
    ~Text() = default;

    sf::Text *GetText() const;

protected:
    sf::Text *m_text;
};
