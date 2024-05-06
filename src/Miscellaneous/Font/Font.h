#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Font
{

public:
    Font(const uint8_t ID, unique_ptr<sf::Font> font);
    ~Font() = default;

    uint8_t GetID() const;
    const sf::Font *GetFont() const;

private:
    uint8_t m_ID;
    unique_ptr<sf::Font> m_font;
};
