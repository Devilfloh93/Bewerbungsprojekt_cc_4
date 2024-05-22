#include "Collision.h"

Collision::Collision()
{
}

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

void Collision::CheckCollision(Game *game)
{
    bool objectInFront = false;
    auto world = game->GetWorld();
    auto creature = game->GetCreature();
    auto player = game->GetPlayer();

    for (const auto &data1 : world)
    {
        auto worldSprite = data1->GetSprite();
        auto worldObjPos = worldSprite->getPosition();

        if (InViewRange(game, worldObjPos))
        {
            auto worldObjCollision = data1->GetCollision();
            auto worldObjSize = worldSprite->getLocalBounds().getSize();
            auto worldInteractable = data1->GetInteractable();

            if (worldObjCollision.x != 0 && worldObjCollision.y != 0)
                objectInFront = CheckInFront(player, worldInteractable, worldObjPos, worldObjSize, worldObjCollision);
            else
                objectInFront = CheckInFront(player, worldInteractable, worldObjPos, worldObjSize);

            if (objectInFront)
            {
                player->SetWorldObjInFront(data1);
                break;
            }
            else
                player->SetWorldObjInFront(nullptr);
        }
    }

    for (const auto &data : creature)
    {
        auto creatureSprite = data->GetSprite();
        auto creatureObjPos = creatureSprite->getPosition();

        if (InViewRange(game, creatureObjPos))
        {
            auto creatureObjSize = creatureSprite->getLocalBounds().getSize();
            auto creatureInteractable = data->GetInteractable();

            objectInFront = CheckInFront(player, creatureInteractable, creatureObjPos, creatureObjSize);

            if (objectInFront)
            {
                player->SetCreatureObjInFront(data);
                break;
            }
            else
                player->SetCreatureObjInFront(nullptr);
        }
    }
}

void Collision::CheckCollision(Game *game, Creature *creature)
{
    auto world = game->GetWorld();
    auto creatures = game->GetCreature();
    auto playerSprite = game->GetPlayer()->GetSprite();
    auto playerObjPos = playerSprite->getPosition();

    if (InViewRange(game, playerObjPos))
    {
        auto playerObjSize = playerSprite->getLocalBounds().getSize();

        CheckInFront(creature, playerObjPos, playerObjSize);
    }

    for (const auto &data1 : world)
    {
        auto worldSprite = data1->GetSprite();
        auto worldObjPos = worldSprite->getPosition();

        if (InViewRange(game, worldObjPos))
        {
            auto worldObjCollision = data1->GetCollision();
            auto worldObjSize = worldSprite->getLocalBounds().getSize();

            if (worldObjCollision.x != 0 && worldObjCollision.y != 0)
                CheckInFront(creature, worldObjPos, worldObjSize, worldObjCollision);
            else
                CheckInFront(creature, worldObjPos, worldObjSize);
        }
    }

    for (const auto &data : creatures)
    {
        auto creatureSprite = data->GetSprite();
        auto creatureObjPos = creatureSprite->getPosition();

        if (InViewRange(game, creatureObjPos) && creature != data)
        {
            auto creatureObjSize = creatureSprite->getLocalBounds().getSize();

            CheckInFront(creature, creatureObjPos, creatureObjSize);
        }
    }
}

bool Collision::CheckInFront(Player *player,
                             const bool interactable,
                             const sf::Vector2f &objPos,
                             const sf::Vector2f &objSize,
                             const CollisionData objCollision)
{
    auto spritePos = player->GetSprite()->getPosition();
    auto speed = player->GetSpeed();
    auto lastMove = player->GetLastMove();
    auto move = player->GetMove();

    auto canMoveDownUpX = CanMoveDownUpX(spritePos.x, objPos.x, objCollision.x);
    auto canMoveUpY = CanMoveUpY(spritePos.y, objPos.y, objSize.y, objCollision.y, speed);

    if (!canMoveDownUpX && !canMoveUpY)
    {
        player->SetMoveAllowed(Move::Up, false);
        if (interactable && (lastMove == Move::Up || move == Move::Up))
            return true;
    }

    auto canMoveDownY = CanMoveDownY(spritePos.y, objPos.y, objCollision.y, speed);

    if (!canMoveDownUpX && !canMoveDownY)
    {
        player->SetMoveAllowed(Move::Down, false);
        if (interactable && (lastMove == Move::Down || move == Move::Down))
            return true;
    }

    auto canMoveLeftX = CanMoveLeftX(spritePos.x, objPos.x, objCollision.x, speed);
    auto canMoveRightLeftY = CanMoveRightLeftY(spritePos.y, objPos.y, objSize.y, objCollision.y);

    if (!canMoveLeftX && !canMoveRightLeftY)
    {
        player->SetMoveAllowed(Move::Left, false);
        if (interactable && (lastMove == Move::Left || move == Move::Left))
            return true;
    }

    auto canMoveRightX = CanMoveRightX(spritePos.x, objPos.x, speed);

    if (!canMoveRightX && !canMoveRightLeftY)
    {
        player->SetMoveAllowed(Move::Right, false);
        if (interactable && (lastMove == Move::Right || move == Move::Right))
            return true;
    }

    return false;
}

bool Collision::CheckInFront(Player *player,
                             const bool interactable,
                             const sf::Vector2f &objPos,
                             const sf::Vector2f &objSize)
{
    auto sprite = player->GetSprite();
    auto spritePos = sprite->getPosition();
    auto spriteSize = sprite->getLocalBounds().getSize();

    auto speed = player->GetSpeed();
    auto lastMove = player->GetLastMove();
    auto move = player->GetMove();

    auto canMoveDownUpX = CanMoveDownUpX(spritePos.x, spriteSize.x, objPos.x, objSize.x);
    auto canMoveUpY = CanMoveUpY(spritePos.y, spriteSize.y, objPos.y, objSize.y, speed);

    if (!canMoveDownUpX && !canMoveUpY)
    {
        player->SetMoveAllowed(Move::Up, false);
        if (interactable && (lastMove == Move::Up || move == Move::Up))
            return true;
    }

    auto canMoveDownY = CanMoveDownY(spritePos.y, spriteSize.y, objPos.y, objSize.y, speed);

    if (!canMoveDownUpX && !canMoveDownY)
    {
        player->SetMoveAllowed(Move::Down, false);
        if (interactable && (lastMove == Move::Down || move == Move::Down))
            return true;
    }

    auto canMoveLeftX = CanMoveLeftX(spritePos.x, objPos.x, objSize.x, speed);
    auto canMoveRightLeftY = CanMoveRightLeftY(spritePos.y, spriteSize.y, objSize.y, objPos.y);

    if (!canMoveLeftX && !canMoveRightLeftY)
    {
        player->SetMoveAllowed(Move::Left, false);
        if (interactable && (lastMove == Move::Left || move == Move::Left))
            return true;
    }

    auto canMoveRightX = CanMoveRightX(spritePos.x, spriteSize.x, objPos.x, speed);

    if (!canMoveRightX && !canMoveRightLeftY)
    {
        player->SetMoveAllowed(Move::Right, false);
        if (interactable && (lastMove == Move::Right || move == Move::Right))
            return true;
    }

    return false;
}

void Collision::CheckInFront(Creature *creature,
                             const sf::Vector2f &objPos,
                             const sf::Vector2f &objSize,
                             const CollisionData objCollision)
{
    auto spritePos = creature->GetSprite()->getPosition();

    auto speed = creature->GetSpeed();
    auto maxMoveRange = creature->GetMaxMoveRange();
    auto spawnPos = creature->GetSpawnPos();

    auto canMoveDownUpX = CanMoveDownUpX(spritePos.x, objPos.x, objCollision.x);
    auto canMoveUpY = CanMoveUpY(spritePos.y, objPos.y, objSize.y, objCollision.y, speed);
    auto canMoveDownY = CanMoveDownY(spritePos.y, objPos.y, objCollision.y, speed);
    auto canMoveLeftX = CanMoveLeftX(spritePos.x, objPos.x, objCollision.x, speed);
    auto canMoveRightLeftY = CanMoveRightLeftY(spritePos.y, objPos.y, objSize.y, objCollision.y);
    auto canMoveRightX = CanMoveRightX(spritePos.x, objPos.x, speed);

    if (!canMoveDownUpX && !canMoveUpY || !(spritePos.y - (spawnPos.y - maxMoveRange) >= 0))
        creature->SetMoveAllowed(Move::Up, false);

    if (!canMoveDownUpX && !canMoveDownY || !(spritePos.y - spawnPos.y <= maxMoveRange))
        creature->SetMoveAllowed(Move::Down, false);

    if (!canMoveLeftX && !canMoveRightLeftY || !(spritePos.x - (spawnPos.x - maxMoveRange) >= 0))
        creature->SetMoveAllowed(Move::Left, false);

    if (!canMoveRightX && !canMoveRightLeftY || !(spritePos.x - spawnPos.x <= maxMoveRange))
        creature->SetMoveAllowed(Move::Right, false);
}

void Collision::CheckInFront(Creature *creature, const sf::Vector2f &objPos, const sf::Vector2f &objSize)
{
    auto sprite = creature->GetSprite();
    auto spritePos = sprite->getPosition();
    auto spriteSize = sprite->getLocalBounds().getSize();

    auto speed = creature->GetSpeed();
    auto maxMoveRange = creature->GetMaxMoveRange();
    auto spawnPos = creature->GetSpawnPos();

    auto canMoveDownUpX = CanMoveDownUpX(spritePos.x, spriteSize.x, objPos.x, objSize.x);
    auto canMoveUpY = CanMoveUpY(spritePos.y, spriteSize.y, objPos.y, objSize.y, speed);
    auto canMoveDownY = CanMoveDownY(spritePos.y, spriteSize.y, objPos.y, objSize.y, speed);
    auto canMoveLeftX = CanMoveLeftX(spritePos.x, objPos.x, objSize.x, speed);
    auto canMoveRightLeftY = CanMoveRightLeftY(spritePos.y, spriteSize.y, objSize.y, objPos.y);
    auto canMoveRightX = CanMoveRightX(spritePos.x, spriteSize.x, objPos.x, speed);

    if (!canMoveDownUpX && !canMoveUpY || !(spritePos.y - (spawnPos.y - maxMoveRange) >= 0))
        creature->SetMoveAllowed(Move::Up, false);

    if (!canMoveDownUpX && !canMoveDownY || !(spritePos.y - spawnPos.y <= maxMoveRange))
        creature->SetMoveAllowed(Move::Down, false);

    if (!canMoveLeftX && !canMoveRightLeftY || !(spritePos.x - (spawnPos.x - maxMoveRange) >= 0))
        creature->SetMoveAllowed(Move::Left, false);

    if (!canMoveRightX && !canMoveRightLeftY || !(spritePos.x - spawnPos.x <= maxMoveRange))
        creature->SetMoveAllowed(Move::Right, false);
}
