#include "Unit.h"

Unit::Unit(sf::Sprite *sprite, const float health, const float speed) : Sprite(sprite), m_health(health), m_speed(speed)
{
}

void Unit::SetSpeed(const float speed)
{
    m_speed = speed;
}
