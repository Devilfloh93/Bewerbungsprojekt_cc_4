#pragma once
#include "Sprite.h"

struct MoveAllowed
{
    bool up;
    bool down;
    bool left;
    bool right;
};

enum class Move
{
    NotMoving = 0,
    Up,
    Down,
    Left,
    Right
};

class Unit : public Sprite
{
public:
    Unit(unique_ptr<sf::Sprite> sprite, const float health, const float speed, const uint8_t animID);
    ~Unit() = default;

    void SetSpeed(const float speed);
    float GetSpeed() const;

    uint8_t GetAnimID() const;

    Move GetMove() const;
    Move GetLastMove() const;
    void SetMove(const Move move);
    void SetMoveAllowed(const Move move, const bool moveAllowed);
    MoveAllowed GetMoveAllowed() const;
    void ResetMoveAllowed();

protected:
    float m_health;
    float m_speed;
    // ANIM
    uint8_t m_animID;
    // MOVE
    Move m_move;
    Move m_lastMove;
    MoveAllowed m_moveAllowed;
};
