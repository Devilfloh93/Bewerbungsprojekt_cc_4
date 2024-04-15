#include "Player.h"
#include "Stats.h"
#include "Utilities.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

Player::Player(sf::Sprite *sprite, const uint8_t animID, const string_view name, const uint8_t id)
    : Sprite(sprite), m_animID(animID), m_name(name), m_ID(id)
{
    Init();
}

Player::Player(sf::Sprite *sprite, const uint8_t animID, const uint8_t id) : Sprite(sprite), m_animID(animID), m_ID(id)
{
    Init();
}

// INIT
void Player::Init()
{
    m_sprite->setPosition(80.0F, 80.0F);
    m_survivalStats = {.health = 100.0F, .water = 100.0F, .food = 100.0F};
    m_baseSpeed = 1.0F;
    m_speed = m_baseSpeed;
    m_move = PlayerMove::NotMoving;
    m_lastMove = m_move;
    m_objectInFront = nullptr;
}

// INFO
uint8_t Player::GetID() const
{
    return m_ID;
}

// MOVE
PlayerMove Player::GetMove() const
{
    return m_move;
}


PlayerMove Player::GetLastMove() const
{
    return m_lastMove;
}

void Player::SetMove(const PlayerMove move)
{
    m_lastMove = m_move;
    m_move = move;
}

void Player::SetSpeed(const float speed)
{
    m_speed = speed;
}

void Player::HandleMove(sf::Clock &clock, Game &game)
{
    auto items = game.GetItem();
    auto anim = game.GetAnim();
    auto elapsed = clock.getElapsedTime();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();
    auto tileSize = game.GetTileSize();
    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();
    MovementTexture moveAnim;

    for (const auto &data : anim)
    {
        if (data->GetTextureID() == m_animID)
        {
            moveAnim = data->GetMoveAnim();
            break;
        }
    }

    if (!items.empty())
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            auto sprite = items[i]->GetSprite();
            auto ID = items[i]->GetID();
            auto count = items[i]->GetCount();
            auto itemPos = sprite->getPosition();
            auto itemSize = sprite->getLocalBounds().getSize();

            bool remove = false;
            bool newItem = true;

            if ((playerPos.x + (playerSize.x / 2)) >= itemPos.x && playerPos.x <= itemPos.x + (itemSize.x / 2) &&
                playerPos.y + (playerSize.y / 2) >= itemPos.y && playerPos.y <= itemPos.y + (itemSize.y / 2))
            {
                for (const auto &[key, value] : m_items)
                {
                    if (key == ID)
                    {
                        m_items[key] = value + count;
                        newItem = false;
                    }
                }

                if (newItem)
                {
                    m_items[ID] = count;
                }
                remove = true;
            }

            if (remove)
            {
                game.RemoveItems(i);
                break;
            }
        }
    }


    switch (m_move)
    {
    case PlayerMove::Left:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.left01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.left00);
        else
            clock.restart();

        if (playerPos.x - m_speed > 0 + (tileSize / 2) && m_moveAllowed.left)
            m_sprite->setPosition(playerPos.x - m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.left00);
        break;
    case PlayerMove::Right:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.right01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.right00);
        else
            clock.restart();

        if (playerPos.x + m_speed < width - tileSize && m_moveAllowed.right)
            m_sprite->setPosition(playerPos.x + m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.right00);
        break;
    case PlayerMove::Down:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.down01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.down02);
        else
            clock.restart();

        if (playerPos.y + m_speed < height - tileSize && m_moveAllowed.down)
            m_sprite->setPosition(playerPos.x, playerPos.y + m_speed);
        else
            m_sprite->setTextureRect(moveAnim.down00);

        break;
    case PlayerMove::Up:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.up01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.up02);
        else
            clock.restart();

        if (playerPos.y - m_speed > 0 + (tileSize / 2) && m_moveAllowed.up)
            m_sprite->setPosition(playerPos.x, playerPos.y - m_speed);
        else
            m_sprite->setTextureRect(moveAnim.up00);

        break;
    case PlayerMove::NotMoving:
        switch (m_lastMove)
        {
        case PlayerMove::Down:
            m_sprite->setTextureRect(moveAnim.down00);
            break;
        case PlayerMove::Up:
            m_sprite->setTextureRect(moveAnim.up00);
            break;
        case PlayerMove::Left:
            m_sprite->setTextureRect(moveAnim.left00);
            break;
        case PlayerMove::Right:
            m_sprite->setTextureRect(moveAnim.right00);
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    m_moveAllowed.up = true;
    m_moveAllowed.down = true;
    m_moveAllowed.left = true;
    m_moveAllowed.right = true;
}

// STATS
float Player::GetStatValue(const StatType type) const
{
    float value = 0.0F;
    switch (type)
    {
    case StatType::Health:
        value = m_survivalStats.health;
        break;
    case StatType::Food:
        value = m_survivalStats.food;
        break;
    case StatType::Water:
        value = m_survivalStats.water;
        break;

    default:
        break;
    }

    return value;
}

void Player::UpdateStats(const Game &game)
{
    auto statDecay = game.GetStatDecay();

    if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
    {
        if (m_survivalStats.food - statDecay.food < 0U)
        {
            m_survivalStats.water = 0;
        }
        else
        {
            m_survivalStats.food -= statDecay.food;
        }
        if (m_survivalStats.water - statDecay.water < 0U)
        {
            m_survivalStats.water = 0;
        }
        else
        {
            m_survivalStats.water -= statDecay.water;
        }
    }
}


// ITEMS
void Player::UseItem(Game &game)
{
    auto itemCfg = game.GetItemCfg();
    auto playerPos = m_sprite->getPosition();
    sf::Vector2f itemPos = {0.0F, 0.0F};

    if (m_objectInFront != nullptr)
    {
        auto useable = m_objectInFront->GetUseable();

        if (useable)
        {
            auto itemOutputID = m_objectInFront->GetItemOutputID();


            m_objectInFront->UpdatePosition();
            m_objectInFront->UpdateTextureRect();
            m_objectInFront->SetUseable(false);

            random_device rd;  // a seed source for the random number engine
            mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
            uint16_t posIncrease = 0;
            for (const auto &data : itemCfg)
            {
                auto texture = data->GetTexture();
                auto textureData = data->GetTextureData();
                auto itemID = data->GetID();
                auto maxDrop = data->GetMaxDrop();

                // Random Drop Count
                uniform_int_distribution<> dist(1U, maxDrop);

                for (const auto &data1 : itemOutputID)
                {
                    if (data1 == itemID)
                    {
                        posIncrease += 20;
                        switch (m_move)
                        {
                        case PlayerMove::Up:
                            itemPos = {playerPos.x, playerPos.y + posIncrease};
                            break;
                        case PlayerMove::Down:
                            itemPos = {playerPos.x, playerPos.y - posIncrease};
                            break;
                        case PlayerMove::Right:
                            itemPos = {playerPos.x - posIncrease, playerPos.y};
                            break;
                        case PlayerMove::Left:
                            itemPos = {playerPos.x + posIncrease, playerPos.y};
                            break;
                        case PlayerMove::NotMoving:
                            switch (m_lastMove)
                            {
                            case PlayerMove::Up:
                                itemPos = {playerPos.x, playerPos.y + posIncrease};
                                break;
                            case PlayerMove::Down:
                                itemPos = {playerPos.x, playerPos.y - posIncrease};
                                break;
                            case PlayerMove::Right:
                                itemPos = {playerPos.x - posIncrease, playerPos.y};
                                break;
                            case PlayerMove::Left:
                                itemPos = {playerPos.x + posIncrease, playerPos.y};
                                break;
                            default:
                                break;
                            }
                            break;

                        default:
                            break;
                        }

                        auto itemSprite = new sf::Sprite();
                        itemSprite->setTexture(*texture);
                        itemSprite->setTextureRect(textureData);
                        itemSprite->setPosition(itemPos.x, itemPos.y);

                        auto item = new Item(itemSprite, itemID, dist(gen));
                        game.SetItems(item);
                    }
                }
            }
        }
    }
}

// DRAW
void Player::DrawStats(sf::RenderWindow &window, const Game &game)
{
    auto width = (game.GetView().getCenter().x - (game.GetWindowZoomWidth() / 2)) + 5.0F;

    size_t i = 1;
    size_t j = 1;

    for (const auto &data : game.GetStats())
    {
        auto textureSize = data->GetTextureSize();
        auto type = data->GetType();
        auto sprite = data->GetSprite();
        auto height = (game.GetView().getCenter().y + (game.GetWindowZoomHeight() / 2)) - 10.0F;
        auto textureRectSize = sprite->getTextureRect().getSize();
        auto textureRectPos = sprite->getTextureRect().getPosition();

        if (type == StatType::Empty)
        {
            height -= i * 10;
            ++i;
        }
        else
        {
            auto statValue = GetStatValue(type);
            uint8_t newTextureRectSizeX = (statValue * textureSize.x) / 100;
            if (textureRectSize.x > 0)
            {
                auto textureRect =
                    sf::IntRect{textureRectPos.x, textureRectPos.y, newTextureRectSizeX, textureRectSize.y};
                sprite->setTextureRect(textureRect);
            }
            height -= j * 10;
            ++j;
        }

        sprite->setPosition(sf::Vector2f{width, height});
        window.draw(*sprite);
    }
}

void Player::DrawInventoryItems(sf::RenderWindow &window, const vector<ItemCfg *> &itemCfg)
{
    bool firstIcon = true;

    Utilities utilities;
    sf::Sprite prevSprite;

    sf::Font font;
    font.loadFromFile("ressources/font/Round9x13.ttf");

    for (size_t i = 0; const auto &[key, value] : m_items)
    {
        sf::Sprite itemSprite;
        sf::Text itemText;
        for (const auto &data : itemCfg)
        {
            auto texture = data->GetTexture();
            auto textureData = data->GetTextureData();
            auto ID = data->GetID();
            if (key == ID)
            {
                itemText.setFont(font);
                itemText.setCharacterSize(15);
                itemText.setString(format("{}", value));

                itemSprite.setTexture(*texture);
                itemSprite.setTextureRect(textureData);

                if (firstIcon)
                {
                    utilities.SetTextBeforeIcon(300, 200, itemSprite, itemText);
                    prevSprite = itemSprite;
                    firstIcon = false;
                }
                else
                {
                    utilities.SetTextBeforeIcon(itemSprite, itemText, prevSprite);
                }

                break;
            }
        }
        window.draw(itemText);
        window.draw(itemSprite);
    }
}


// RIGHT MOVE X
bool InRangeOfXRight(const float playerPos, const float playerSize, const float objPos, const float speed)
{
    if (playerPos + playerSize + speed >= objPos && playerPos <= objPos)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionXRight(const float playerPos, const float objPos, const float speed)
{
    if (playerPos + speed >= objPos && playerPos <= objPos)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionYRightLeft(const float playerPos,
                                  const float objPos,
                                  const float objSize,
                                  const uint8_t objCollision)
{
    if (playerPos <= objPos + objSize && playerPos >= objPos + objCollision)
    {
        return true;
    }
    return false;
}

// LEFT RIGHT MOVE Y
bool InRangeOfYRightLeft(const float playerPos, const float playerSize, const float objSize, const float objPos)
{
    if (playerPos <= objPos + objSize && playerPos + playerSize >= objPos)
    {
        return true;
    }
    return false;
}

// LEFT MOVE X
bool InRangeOfXLeft(const float playerPos, const float objPos, const float objSize, const float speed)
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objSize)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionXLeft(const float playerPos, const float objPos, const uint8_t objCollision, const float speed)
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objCollision)
    {
        return true;
    }
    return false;
}

// DOWN - UP MOVE X
bool InRangeOfXDownUp(const float playerPos, const float playerSize, const float objPos, const float objSize)
{
    if (playerPos + playerSize >= objPos && playerPos <= objPos + objSize)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionXDownUp(const float playerPos, const float objPos, const uint8_t objCollision)
{
    if (playerPos >= objPos && playerPos <= objPos + objCollision)
    {
        return true;
    }
    return false;
}

// UP
bool InRangeOfYUp(const float playerPos,
                  const float playerSize,
                  const float objPos,
                  const float objSize,
                  const float speed)
{
    if (playerPos - speed <= objPos + objSize && playerPos + playerSize >= objPos)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionYUp(const float playerPos,
                           const float objPos,
                           const float objSize,
                           const uint8_t objCollision,
                           const float speed)
{
    if (playerPos - speed <= objPos + objSize && playerPos >= objPos + objCollision)
    {
        return true;
    }
    return false;
}

// DOWN
bool InRangeOfYDown(const float playerPos,
                    const float playerSize,
                    const float objPos,
                    const float objSize,
                    const float speed)
{
    if (playerPos <= objPos + objSize && (playerPos + playerSize) + speed >= objPos)
    {
        return true;
    }
    return false;
}

bool InRangeOfCollisionYDown(const float playerPos, const float objPos, const uint8_t objCollision, const float speed)
{
    if (playerPos <= objPos + objCollision && playerPos + speed >= objPos + objCollision)
    {
        return true;
    }
    return false;
}

// COLLISION
void Player::CheckCollision(Game &game)
{
    m_objectInFront = nullptr;

    auto world = game.GetWorld();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    for (const auto &data : world)
    {
        auto objPos = data->GetSprite()->getPosition();
        auto objCollision = data->GetCollision();
        auto objSize = data->GetSprite()->getLocalBounds().getSize();
        auto isUsable = data->GetUseable();

        auto inRangeOfCollisionXDownUP = InRangeOfCollisionXDownUp(playerPos.x, objPos.x, objCollision.x);
        auto inRangeOfCollisionYUp = InRangeOfCollisionYUp(playerPos.y, objPos.y, objSize.y, objCollision.y, m_speed);
        auto inRangeOfCollisionYDown = InRangeOfCollisionYDown(playerPos.y, objPos.y, objCollision.y, m_speed);
        auto inRangeOfCollisionXLeft = InRangeOfCollisionXLeft(playerPos.x, objPos.x, objCollision.x, m_speed);
        auto inRangeOfCollisionYRightLeft =
            InRangeOfCollisionYRightLeft(playerPos.y, objPos.y, objSize.y, objCollision.y);
        auto inRangeOfCollisionXRight = InRangeOfCollisionXRight(playerPos.x, objPos.x, m_speed);
        auto inRangeOfXDownUp = InRangeOfXDownUp(playerPos.x, playerSize.x, objPos.x, objSize.x);
        auto inRangeOfYUp = InRangeOfYUp(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);
        auto inRangeOfYDown = InRangeOfYDown(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);
        auto inRangeOfXLeft = InRangeOfXLeft(playerPos.x, objPos.x, objSize.x, m_speed);
        auto inRangeOfYRightLeft = InRangeOfYRightLeft(playerPos.y, playerSize.y, objSize.y, objPos.y);
        auto inRangeOfXRight = InRangeOfXRight(playerPos.x, playerSize.x, objPos.x, m_speed);

        if (objCollision.x != 0 && objCollision.y != 0)
        {
            if (inRangeOfCollisionXDownUP && inRangeOfCollisionYUp)
            {
                m_moveAllowed.up = false;
                if (isUsable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
                {
                    m_objectInFront = data;
                }
            }

            if (inRangeOfCollisionXDownUP && inRangeOfCollisionYDown)
            {
                m_moveAllowed.down = false;
                if (isUsable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
                {
                    m_objectInFront = data;
                }
            }

            if (inRangeOfCollisionXLeft && inRangeOfCollisionYRightLeft)
            {
                m_moveAllowed.left = false;
                if (isUsable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
                {
                    m_objectInFront = data;
                }
            }


            if (inRangeOfCollisionXRight && inRangeOfCollisionYRightLeft)
            {
                m_moveAllowed.right = false;
                if (isUsable && (m_lastMove == PlayerMove::Right || m_move == PlayerMove::Right))
                {
                    m_objectInFront = data;
                }
            }
        }
        else
        {
            if (inRangeOfXDownUp && inRangeOfYUp)
            {
                m_moveAllowed.up = false;
                if (isUsable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
                {
                    m_objectInFront = data;
                }
            }

            if (inRangeOfXDownUp && inRangeOfYDown)
            {
                m_moveAllowed.down = false;
                if (isUsable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
                {
                    m_objectInFront = data;
                }
            }

            if (inRangeOfXLeft && inRangeOfYRightLeft)
            {
                m_moveAllowed.left = false;
                if (isUsable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
                {
                    m_objectInFront = data;
                }
            }

            if (inRangeOfXRight && inRangeOfYRightLeft)
            {
                m_moveAllowed.right = false;
                if (isUsable && (m_lastMove == PlayerMove::Right || m_move == PlayerMove::Right))
                {
                    m_objectInFront = data;
                }
            }
        }
    }
}

// DATASTORE
void Player::Load(const uint8_t id)
{
    auto path = format("./save/{}/save.json", id);

    ifstream file(path);

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        m_name = jsonData["name"];
        m_survivalStats.health = jsonData["health"];
        m_survivalStats.water = jsonData["water"];
        m_survivalStats.food = jsonData["food"];
        m_sprite->setPosition(jsonData["posX"], jsonData["posY"]);

        for (const auto &data : jsonData["items"])
        {
            m_items[data[0]] = data[1];
        }

        file.close();
    }
}

void Player::Save()
{
    auto path = format("./save/{}/save.json", m_ID);

    ofstream file(path);

    if (file.is_open())
    {
        json jsonData = {{"name", m_name},
                         {"health", m_survivalStats.health},
                         {"water", m_survivalStats.water},
                         {"food", m_survivalStats.food},
                         {"posX", m_sprite->getPosition().x},
                         {"posY", m_sprite->getPosition().y},
                         {"items", m_items}};
        file << jsonData;
        file.close();
    }
}
