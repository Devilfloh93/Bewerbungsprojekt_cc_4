#include "Collision.h"

Collision::Collision()
{
}

// RIGHT MOVE X
bool Collision::CanMoveRightX(const float playerPos,
                              const float playerSize,
                              const float objPos,
                              const float speed) const
{
    if (playerPos + playerSize + speed >= objPos && playerPos <= objPos)
        return false;
    return true;
}

bool Collision::CanMoveRightX(const float playerPos, const float objPos, const float speed) const
{
    if (playerPos + speed >= objPos && playerPos <= objPos)
        return false;
    return true;
}

// RIGHT LEFT MOVE Y
bool Collision::CanMoveRightLeftY(const float playerPos,
                                  const float playerSize,
                                  const float objSize,
                                  const float objPos) const
{
    if (playerPos <= objPos + objSize && playerPos + playerSize >= objPos)
        return false;
    return true;
}

bool Collision::CanMoveRightLeftY(const float playerPos,
                                  const float objPos,
                                  const float objSize,
                                  const uint8_t objCollision) const
{
    if (playerPos <= objPos + objSize && playerPos >= objPos + objCollision)
        return false;
    return true;
}

// LEFT MOVE X
bool Collision::CanMoveLeftX(const float playerPos, const float objPos, const float objSize, const float speed) const
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objSize)
        return false;
    return true;
}

bool Collision::CanMoveLeftX(const float playerPos,
                             const float objPos,
                             const uint8_t objCollision,
                             const float speed) const
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objCollision)
        return false;
    return true;
}

// DOWN UP MOVE X
bool Collision::CanMoveDownUpX(const float playerPos,
                               const float playerSize,
                               const float objPos,
                               const float objSize) const
{
    if (playerPos + playerSize >= objPos && playerPos <= objPos + objSize)
        return false;
    return true;
}

bool Collision::CanMoveDownUpX(const float playerPos, const float objPos, const uint8_t objCollision) const
{
    if (playerPos >= objPos && playerPos <= objPos + objCollision)
        return false;
    return true;
}

// UP MOVE
bool Collision::CanMoveUpY(const float playerPos,
                           const float playerSize,
                           const float objPos,
                           const float objSize,
                           const float speed) const
{
    if (playerPos - speed <= objPos + objSize && playerPos + playerSize >= objPos)
        return false;
    return true;
}

bool Collision::CanMoveUpY(const float playerPos,
                           const float objPos,
                           const float objSize,
                           const uint8_t objCollision,
                           const float speed) const
{
    if (playerPos - speed <= objPos + objSize && playerPos >= objPos + objCollision)
        return false;
    return true;
}

// DOWN MOVE
bool Collision::CanMoveDownY(const float playerPos,
                             const float playerSize,
                             const float objPos,
                             const float objSize,
                             const float speed) const
{
    if (playerPos <= objPos + objSize && (playerPos + playerSize) + speed >= objPos)
        return false;
    return true;
}

bool Collision::CanMoveDownY(const float playerPos,
                             const float objPos,
                             const uint8_t objCollision,
                             const float speed) const
{
    if (playerPos <= objPos + objCollision && playerPos + speed >= objPos + objCollision)
        return false;
    return true;
}

bool Collision::InViewRange(Game *game, const sf::Vector2f &spritePos)
{
    auto renderPuffer = game->GetRenderPuffer();
    auto view = game->GetView();

    auto viewCenter = view->getCenter();
    auto viewSizeX = ((view->getSize().x / 2) + renderPuffer);
    auto viewSizeY = ((view->getSize().y / 2) + renderPuffer);

    if (spritePos.x >= (viewCenter.x - viewSizeX) && spritePos.x <= (viewCenter.x + viewSizeX) &&
        spritePos.y >= (viewCenter.y - viewSizeY) && spritePos.y <= (viewCenter.y + viewSizeY))
    {
        return true;
    }
    return false;
}
