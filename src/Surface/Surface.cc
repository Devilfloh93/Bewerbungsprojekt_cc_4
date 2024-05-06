#include "Surface.h"


Surface::Surface(unique_ptr<sf::Sprite> sprite, const float speed) : Sprite(move(sprite)), m_speed(speed)
{
}

float Surface::GetSpeed() const
{
    return m_speed;
}
