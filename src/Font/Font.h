#pragma once
#include <SFML/Graphics.hpp>

class Font
{

public:
    Font(const std::uint8_t ID, sf::Font *font);
    ~Font() = default;

    std::uint8_t GetID() const;
    sf::Font *GetFont() const;

private:
    std::uint8_t m_ID;
    sf::Font *m_font;
};

void InitFont(std::vector<std::unique_ptr<Font>> &fonts);
