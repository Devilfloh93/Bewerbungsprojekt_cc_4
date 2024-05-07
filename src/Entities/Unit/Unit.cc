#include "Unit.h"

/**
 * @brief Construct a new Unit:: Unit object
 *
 * @param sprite
 * @param health healtpoints the creature has
 * @param speed how many pixels the creature can move with one movement command
 * @param animID
 */

Unit::Unit(unique_ptr<sf::Sprite> sprite, const float health, const float speed, const uint8_t animID)
    : Sprite(move(sprite)), m_health(health), m_speed(speed), m_animID(animID)
{
}

void Unit::SetSpeed(const float speed)
{
    m_speed = speed;
}
