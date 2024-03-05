#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

enum class PlayerMove
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

    PlayerMove GetMovement() const;

    void SetMovement(const PlayerMove movement);
    void SetSpeed(const float speed);

private:
    std::string m_name;
    float m_health;
    float m_water;
    float m_food;
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_movement;
};

void InitPlayer(sf::Sprite &playerSprite, const sf::Texture &texture);
