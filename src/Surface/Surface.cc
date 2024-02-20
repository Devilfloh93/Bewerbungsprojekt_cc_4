#include "Surface.h"


Surface::Surface(const sf::Sprite sprite, const float speed) : m_sprite(sprite), m_speed(speed)
{
}

Grass::Grass(const sf::Sprite sprite, const float speed) : Surface(sprite, speed)
{
}

Water::Water(const sf::Sprite sprite, const float speed) : Surface(sprite, speed)
{
}

sf::Sprite Surface::GetSprite() const
{
    return this->m_sprite;
}

float Surface::GetSpeed() const
{
    return this->m_speed;
}
