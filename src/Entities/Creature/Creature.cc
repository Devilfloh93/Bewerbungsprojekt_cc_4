#include "Creature.h"

Creature::Creature(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID, const bool moving)
    : Unit(sprite, health, speed, animID), m_moving(moving)
{
    m_useable = false;
}

bool Creature::GetUseable() const
{
    return m_useable;
}

bool Creature::GetMoving() const
{
    return m_moving;
}
