#pragma once
#include <cstdint>
#include <string>

enum class PlayerMovement
{
    NotMoving = 0,
    Up,
    Down,
    Left,
    Right
};

class Player
{
public:
    Player(const std::string name, const float health, const float water, const float food, const float baseSpeed);
    ~Player() = default;

    std::string GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;
    float GetBaseSpeed() const;
    float GetSpeed() const;

    PlayerMovement GetMovement() const;

    void SetMovement(const PlayerMovement movement);
    void SetSpeed(const float speed);

private:
    std::string m_name;
    float m_health;
    float m_water;
    float m_food;
    float m_baseSpeed;
    float m_speed = m_baseSpeed;
    PlayerMovement m_movement = PlayerMovement::NotMoving;
};
