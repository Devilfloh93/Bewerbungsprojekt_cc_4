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
