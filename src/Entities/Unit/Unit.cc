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
    m_move = Move::NotMoving;
    m_lastMove = m_move;
}

void Unit::SetSpeed(const float speed)
{
    m_speed = speed;
}

float Unit::GetSpeed() const
{
    return m_speed;
}

// MOVE
Move Unit::GetMove() const
{
    return m_move;
}

Move Unit::GetLastMove() const
{
    return m_lastMove;
}

void Unit::SetMove(const Move move)
{
    m_lastMove = m_move;
    m_move = move;
}

void Unit::SetMoveAllowed(const Move move, const bool moveAllowed)
{
    switch (move)
    {
    case Move::Up:
        m_moveAllowed.up = moveAllowed;
        break;
    case Move::Down:
        m_moveAllowed.down = moveAllowed;
        break;
    case Move::Left:
        m_moveAllowed.left = moveAllowed;
        break;
    case Move::Right:
        m_moveAllowed.right = moveAllowed;
        break;

    default:
        break;
    }
}

MoveAllowed Unit::GetMoveAllowed() const
{
    return m_moveAllowed;
}
