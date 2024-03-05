#include "Player.h"

Player::Player(const std::string name, const float health, const float water, const float food, const float baseSpeed)
    : m_name(name), m_health(health), m_water(water), m_food(food), m_baseSpeed(baseSpeed)
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
    return this->m_health;
}

float Player::GetWater() const
{
    return this->m_water;
}

float Player::GetFood() const
{
    return this->m_food;
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
