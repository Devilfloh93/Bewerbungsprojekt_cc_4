#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Font
{

public:
    Font(const uint8_t ID, sf::Font *font);
    ~Font() = default;

    uint8_t GetID() const;
    sf::Font *GetFont() const;

private:
    uint8_t m_ID;
    sf::Font *m_font;
};
