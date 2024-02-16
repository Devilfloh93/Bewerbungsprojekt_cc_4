#include "Player.h"

Player::Player(const std::string name, const float health, const float water, const float food)
    : m_name(name), m_health(health), m_water(water), m_food(food)
{
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
