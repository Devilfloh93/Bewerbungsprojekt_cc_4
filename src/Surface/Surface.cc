#include "Surface.h"

Surface::Surface(const sf::Sprite &sprite, const float speed) : m_sprite(sprite), m_speed(speed)
{
}

sf::Sprite Surface::GetSprite() const
{
    return m_sprite;
}

float Surface::GetSpeed() const
{
    return m_speed;
}
