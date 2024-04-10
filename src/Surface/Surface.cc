#include "Surface.h"


Surface::Surface(sf::Sprite *sprite, const float speed) : Sprite(sprite), m_speed(speed)
{
}

float Surface::GetSpeed() const
{
    return m_speed;
}
