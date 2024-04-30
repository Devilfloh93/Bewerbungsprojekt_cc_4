#pragma once
#include "Game.h"
#include <cstdint>

using namespace std;

class Collision
{

public:
    Collision();
    ~Collision() = default;

    bool InViewRange(Game *game, const sf::Vector2f &spritePos);

    // RIGHT MOVE X
    bool CanMoveRightX(const float playerPos, const float playerSize, const float objPos, const float speed) const;
    bool CanMoveRightX(const float playerPos, const float objPos, const float speed) const;

    // RIGHT LEFT  MOVE Y
    bool CanMoveRightLeftY(const float playerPos,
                           const float playerSize,
                           const float objSize,
                           const float objPos) const;
    bool CanMoveRightLeftY(const float playerPos,
                           const float objPos,
                           const float objSize,
                           const uint8_t objCollision) const;

    // LEFT MOVE X
    bool CanMoveLeftX(const float playerPos, const float objPos, const float objSize, const float speed) const;
    bool CanMoveLeftX(const float playerPos, const float objPos, const uint8_t objCollision, const float speed) const;

    // DOWN UP MOVE X
    bool CanMoveDownUpX(const float playerPos, const float playerSize, const float objPos, const float objSize) const;
    bool CanMoveDownUpX(const float playerPos, const float objPos, const uint8_t objCollision) const;

    // UP MOVE
    bool CanMoveUpY(const float playerPos,
                    const float playerSize,
                    const float objPos,
                    const float objSize,
                    const float speed) const;
    bool CanMoveUpY(const float playerPos,
                    const float objPos,
                    const float objSize,
                    const uint8_t objCollision,
                    const float speed) const;

    // DOWN MOVE
    bool CanMoveDownY(const float playerPos,
                      const float playerSize,
                      const float objPos,
                      const float objSize,
                      const float speed) const;
    bool CanMoveDownY(const float playerPos, const float objPos, const uint8_t objCollision, const float speed) const;
};
