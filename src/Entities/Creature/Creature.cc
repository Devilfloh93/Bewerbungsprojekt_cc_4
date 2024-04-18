#include "Creature.h"

Creature::Creature(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID)
    : Unit(sprite, health, speed, animID)
{
    m_useable = false;
}

bool Creature::GetUseable() const
{
    return m_useable;
}
