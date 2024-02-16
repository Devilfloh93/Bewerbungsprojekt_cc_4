#pragma once
#include <cstdint>
#include <string>
class Player
{

public:
    Player(const std::string name, const float health, const float water, const float food);
    ~Player() = default;

    std::string GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;

private:
    std::string m_name;
    float m_health;
    float m_water;
    float m_food;
};
