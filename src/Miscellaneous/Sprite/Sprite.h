#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Sprite
{
public:
    Sprite(unique_ptr<sf::Sprite> sprite);
    ~Sprite() = default;

    sf::Sprite *GetSprite() const;

protected:
    unique_ptr<sf::Sprite> m_sprite;
};
