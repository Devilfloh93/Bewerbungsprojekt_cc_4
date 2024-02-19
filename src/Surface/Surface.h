#pragma once
#include <SFML/Graphics.hpp>

class Surface
{

public:
    Surface(const sf::Sprite sprite, const bool canPass);
    ~Surface() = default;

    sf::Sprite GetSprite() const;

private:
    sf::Sprite m_sprite;
    bool m_canPass;
};
