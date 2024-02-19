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
    Player(const std::string name, const float health, const float water, const float food);
    ~Player() = default;

    std::string GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;

    PlayerMovement GetMovement() const;

    void SetMovement(const PlayerMovement movement);

private:
    std::string m_name;
    float m_health;
    float m_water;
    float m_food;
    PlayerMovement m_movement = PlayerMovement::NotMoving;
};
