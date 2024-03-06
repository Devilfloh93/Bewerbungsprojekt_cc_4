#include "Player.h"
#include "Game.h"


Player::Player(const std::string name, const PlayerSurvivalStats survivalStats, const float baseSpeed)
    : m_name(name), m_survivalStats(survivalStats), m_baseSpeed(baseSpeed)
{
    m_speed = m_baseSpeed;
    m_movement = PlayerMove::NotMoving;
}

std::string Player::GetName() const
{
    return this->m_name;
}

float Player::GetHealth() const
{
    return this->m_survivalStats.health;
}

float Player::GetWater() const
{
    return this->m_survivalStats.water;
}

float Player::GetFood() const
{
    return this->m_survivalStats.food;
}

PlayerMove Player::GetMovement() const
{
    return this->m_movement;
}

float Player::GetBaseSpeed() const
{
    return this->m_baseSpeed;
}

float Player::GetSpeed() const
{
    return this->m_speed;
}

void Player::SetMovement(const PlayerMove movement)
{
    this->m_movement = movement;
}

void Player::SetSpeed(const float speed)
{
    this->m_speed = speed;
}

void InitPlayer(sf::Sprite &playerSprite, const sf::Texture &texture)
{
    playerSprite.setTexture(texture);
    playerSprite.setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
    playerSprite.setPosition(80.0F, 80.0F);
}

void HandlePlayerMovement(const Player &player,
                          sf::Clock &clock,
                          sf::Sprite &playerSprite,
                          const uint32_t height,
                          const uint32_t width,
                          const uint32_t tileSize)
{
    auto elapsed = clock.getElapsedTime();
    auto speed = player.GetSpeed();
    auto playerPos = playerSprite.getPosition();

    const static auto moveTexture = MovementTexture{.notMoving{8U, 8U, 16U, 16U},
                                                    .up01{72U, 136U, 16U, 16U},
                                                    .up02{136U, 136U, 16U, 16U},
                                                    .down01{72U, 8U, 16U, 16U},
                                                    .down02{136U, 8U, 16U, 16U},
                                                    .left01{72U, 200U, 16U, 16U},
                                                    .left02{8U, 200U, 16U, 16U},
                                                    .right01{72U, 72U, 16U, 16U},
                                                    .right02{8U, 72U, 16U, 16U}};

    switch (player.GetMovement())
    {
    case PlayerMove::Left:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
        {
            // RIGHT FEET
            playerSprite.setTextureRect(moveTexture.left01);
        }
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
        {
            // LEFT FEET
            playerSprite.setTextureRect(moveTexture.left02);
        }
        else
        {
            clock.restart();
        }

        if (playerPos.x - speed > 0 + (tileSize / 2))
        {
            playerSprite.setPosition(playerPos.x - speed, playerPos.y);
        }
        else
        {
            playerSprite.setTextureRect(moveTexture.notMoving);
        }
        break;
    case PlayerMove::Right:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
        {
            // RIGHT FEET
            playerSprite.setTextureRect(moveTexture.right01);
        }
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
        {
            // LEFT FEET
            playerSprite.setTextureRect(moveTexture.right02);
        }
        else
        {
            clock.restart();
        }

        if (playerPos.x + speed < width - tileSize)
        {
            playerSprite.setPosition(playerPos.x + speed, playerPos.y);
        }
        else
        {
            playerSprite.setTextureRect(moveTexture.notMoving);
        }
        break;
    case PlayerMove::Down:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
        {
            // RIGHT FEET
            playerSprite.setTextureRect(moveTexture.down01);
        }
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
        {
            // LEFT FEET
            playerSprite.setTextureRect(moveTexture.down02);
        }
        else
        {
            clock.restart();
        }

        if (playerPos.y + speed < height - tileSize)
        {
            playerSprite.setPosition(playerPos.x, playerPos.y + speed);
        }
        else
        {
            playerSprite.setTextureRect(moveTexture.notMoving);
        }

        break;
    case PlayerMove::Up:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
        {
            // RIGHT FEET
            playerSprite.setTextureRect(moveTexture.up01);
        }
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
        {
            // LEFT FEET
            playerSprite.setTextureRect(moveTexture.up02);
        }
        else
        {
            clock.restart();
        }

        if (playerPos.y - speed > 0 + (tileSize / 2))
        {
            playerSprite.setPosition(playerPos.x, playerPos.y - speed);
        }
        else
        {
            playerSprite.setTextureRect(moveTexture.notMoving);
        }

        break;
    case PlayerMove::NotMoving:
        playerSprite.setTextureRect(moveTexture.notMoving);
        break;

    default:
        break;
    }
}
