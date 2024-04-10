#pragma once
#include <SFML/Graphics.hpp>

class Sprite
{
public:
    Sprite(sf::Sprite *sprite);
    ~Sprite() = default;

    sf::Sprite *GetSprite() const;

protected:
    sf::Sprite *m_sprite;
};
