#include "Player.h"
#include <format>
#include <memory>
#include <random>

Player::Player(sf::Sprite *sprite,
               const std::string_view name,
               const PlayerSurvivalStats survivalStats,
               const float baseSpeed)
    : m_sprite(sprite), m_name(name), m_survivalStats(survivalStats), m_baseSpeed(baseSpeed)
{
    m_speed = m_baseSpeed;
    m_move = PlayerMove::NotMoving;
    m_lastMove = m_move;
}

std::string_view Player::GetName() const
{
    return m_name;
}

float Player::GetHealth() const
{
    return m_survivalStats.health;
}

float Player::GetWater() const
{
    return m_survivalStats.water;
}

float Player::GetFood() const
{
    return m_survivalStats.food;
}

PlayerMove Player::GetMove() const
{
    return m_move;
}

PlayerMove Player::GetLastMove() const
{
    return m_lastMove;
}

float Player::GetBaseSpeed() const
{
    return m_baseSpeed;
}

float Player::GetSpeed() const
{
    return m_speed;
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

sf::Sprite *Player::GetSprite() const
{
    return m_sprite;
}


void Player::DrawInventoryItems(sf::RenderWindow &window, const std::vector<ItemCfg> &itemCfg)
{
    bool firstIcon = true;

    sf::Sprite prevSprite;

    sf::Font font;
    font.loadFromFile("ressources/font/Round9x13.ttf");

    for (size_t i = 0; const auto &[key, value] : m_items)
    {
        sf::Sprite itemSprite;
        sf::Text itemText;
        for (const auto &data : itemCfg)
        {
            auto texture = data.GetTexture();
            auto textureData = data.GetTextureData();
            auto ID = data.GetID();
            if (key == ID)
            {
                itemText.setFont(font);
                itemText.setCharacterSize(15);
                itemText.setString(std::format("{}", value));

                itemSprite.setTexture(*texture);
                itemSprite.setTextureRect(textureData);

                if (firstIcon)
                {
                    Menu::SetTextBeforeIcon(300, 200, itemSprite, itemText);
                    prevSprite = itemSprite;
                    firstIcon = false;
                }
                else
                {
                    Menu::SetTextBeforeIcon(itemSprite, itemText, prevSprite);
                }

                break;
            }
        }
        window.draw(itemText);
        window.draw(itemSprite);
    }
}

void Player::HandleMove(sf::Clock &clock, const Game &game)
{
    auto world = (game.GetWorld());
    auto items = (game.GetItem());
    auto elapsed = clock.getElapsedTime();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();
    auto tileSize = game.GetTileSize();
    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();

    const static auto moveTexture = MovementTexture{.up00{8U, 136U, 16U, 16U},
                                                    .up01{72U, 136U, 16U, 16U},
                                                    .up02{136U, 136U, 16U, 16U},
                                                    .down00{8U, 8U, 16U, 16U},
                                                    .down01{72U, 8U, 16U, 16U},
                                                    .down02{136U, 8U, 16U, 16U},
                                                    .left00{8U, 200U, 16U, 16U},
                                                    .left01{72U, 200U, 16U, 16U},
                                                    .right00{8U, 72U, 16U, 16U},
                                                    .right01{72U, 72U, 16U, 16U}};

    bool canMoveUP = true;
    bool canMoveDOWN = true;
    bool canMoveLEFT = true;
    bool canMoveRIGHT = true;

    for (const auto &data : world)
    {
        auto objPos = data.GetSprite().getPosition();
        auto objCollision = data.GetCollision();
        auto objSize = data.GetSprite().getLocalBounds().getSize();

        if (objCollision.x != 0 && objCollision.y != 0)
        {
            if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
                canMoveUP = false;

            if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                playerPos.y <= objPos.y + objCollision.y && playerPos.y + m_speed >= objPos.y + objCollision.y)
                canMoveDOWN = false;

            if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objCollision.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
                canMoveLEFT = false;

            if (playerPos.x + m_speed >= objPos.x && playerPos.x <= objPos.x && playerPos.y <= objPos.y + objSize.y &&
                playerPos.y >= objPos.y + objCollision.y)
                canMoveRIGHT = false;
        }
        else
        {
            if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                canMoveUP = false;

            if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                playerPos.y <= objPos.y + objSize.y && (playerPos.y + playerSize.y) + m_speed >= objPos.y)
                canMoveDOWN = false;

            if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objSize.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                canMoveLEFT = false;

            if ((playerPos.x + playerSize.x) + m_speed >= objPos.x && playerPos.x <= objPos.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                canMoveRIGHT = false;
        }
    }

    for (auto it = items.begin(); it != items.end();)
    {
        auto ID = it->GetID();
        auto count = it->GetCount();
        auto itemPos = it->GetSprite().getPosition();
        auto itemSize = it->GetSprite().getLocalBounds().getSize();
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
            items.erase(it);
        }
        else
        {
            ++it;
        }
    }

    switch (m_move)
    {
    case PlayerMove::Left:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveTexture.left01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveTexture.left00);
        else
            clock.restart();

        if (playerPos.x - m_speed > 0 + (tileSize / 2) && canMoveLEFT)
            m_sprite->setPosition(playerPos.x - m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveTexture.left00);
        break;
    case PlayerMove::Right:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveTexture.right01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveTexture.right00);
        else
            clock.restart();

        if (playerPos.x + m_speed < width - tileSize && canMoveRIGHT)
            m_sprite->setPosition(playerPos.x + m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveTexture.right00);
        break;
    case PlayerMove::Down:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveTexture.down01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveTexture.down02);
        else
            clock.restart();

        if (playerPos.y + m_speed < height - tileSize && canMoveDOWN)
            m_sprite->setPosition(playerPos.x, playerPos.y + m_speed);
        else
            m_sprite->setTextureRect(moveTexture.down00);

        break;
    case PlayerMove::Up:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveTexture.up01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveTexture.up02);
        else
            clock.restart();

        if (playerPos.y - m_speed > 0 + (tileSize / 2) && canMoveUP)
            m_sprite->setPosition(playerPos.x, playerPos.y - m_speed);
        else
            m_sprite->setTextureRect(moveTexture.up00);

        break;
    case PlayerMove::NotMoving:
        switch (m_lastMove)
        {
        case PlayerMove::Down:
            m_sprite->setTextureRect(moveTexture.down00);
            break;
        case PlayerMove::Up:
            m_sprite->setTextureRect(moveTexture.up00);
            break;
        case PlayerMove::Left:
            m_sprite->setTextureRect(moveTexture.left00);
            break;
        case PlayerMove::Right:
            m_sprite->setTextureRect(moveTexture.right00);
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void Player::UseItem(const Game &game)
{
    auto world = (game.GetWorld());
    auto itemCfg = (game.GetItemCfg());
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    bool useItem = false;
    std::uint8_t itemOutputID = 0U;
    sf::Vector2f itemPos = {0.0F, 0.0F};
    for (auto &data : world)
    {
        auto objSprite = data.GetSprite();
        auto objPos = objSprite.getPosition();
        auto objCollision = data.GetCollision();
        auto objSize = objSprite.getLocalBounds().getSize();
        auto useable = data.GetUseable();
        itemOutputID = data.GetItemOutputID();

        if (useable)
        {
            /**
            * TODO: Check Range to Object / Rework Object Collision
            */
            if (objCollision.x != 0U && objCollision.y != 0U)
            {
                if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                    playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
                {
                    itemPos = {playerPos.x, playerPos.y + 20};
                    useItem = true;
                }

                if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                    playerPos.y <= objPos.y + objCollision.y && playerPos.y + m_speed >= objPos.y + objCollision.y)
                {
                    itemPos = {playerPos.x, playerPos.y - 20};
                    useItem = true;
                }

                if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objCollision.x &&
                    playerPos.y <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
                {
                    itemPos = {playerPos.x + 20, playerPos.y};
                    useItem = true;
                }

                if (playerPos.x + m_speed >= objPos.x && playerPos.x <= objPos.x &&
                    playerPos.y <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
                {
                    itemPos = {playerPos.x - 20, playerPos.y};
                    useItem = true;
                }
            }
            else
            {
                if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                    playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                {
                    itemPos = {playerPos.x, playerPos.y + 20};
                    useItem = true;
                }

                if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                    playerPos.y <= objPos.y + objSize.y && (playerPos.y + playerSize.y) + m_speed >= objPos.y)
                {
                    itemPos = {playerPos.x, playerPos.y - 20};
                    useItem = true;
                }

                if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objSize.x &&
                    playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                {
                    itemPos = {playerPos.x + 20, playerPos.y};
                    useItem = true;
                }

                if ((playerPos.x + playerSize.x) + m_speed >= objPos.x && playerPos.x <= objPos.x &&
                    playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
                {
                    itemPos = {playerPos.x - 20, playerPos.y};
                    useItem = true;
                }
            }

            if (useItem)
            {
                data.UpdatePosition();
                data.UpdateTextureRect();
                data.SetUseable(false);
                break;
            }
        }
    }

    if (useItem)
    {
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        auto items = (game.GetItem());
        for (const auto &data : itemCfg)
        {
            sf::Sprite itemSprite;
            auto texture = data.GetTexture();
            auto textureData = data.GetTextureData();
            auto itemID = data.GetID();
            auto maxDrop = data.GetMaxDrop();

            // Random Drop Count
            std::uniform_int_distribution<> dist(1U, maxDrop);

            if (itemOutputID == itemID)
            {
                itemSprite.setTexture(*texture);
                itemSprite.setTextureRect(textureData);
                itemSprite.setPosition(itemPos.x, itemPos.y);

                auto item = new Item(itemSprite, itemID, dist(gen));
                items.push_back(*item);
                break;
            }
        }
    }
}
