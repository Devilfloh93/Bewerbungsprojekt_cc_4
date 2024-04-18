#include "Unit.h"

Unit::Unit(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID)
    : Sprite(sprite), m_health(health), m_speed(speed), m_animID(animID)
{
}

void Unit::SetSpeed(const float speed)
{
    m_speed = speed;
}
