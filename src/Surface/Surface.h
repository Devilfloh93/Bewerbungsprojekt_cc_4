#pragma once
#include "Sprite.h"
#include <SFML/Graphics.hpp>

class Surface : public Sprite
{

public:
    Surface(sf::Sprite *sprite, const float speed);
    ~Surface() = default;

    float GetSpeed() const;

private:
    float m_speed;
};
