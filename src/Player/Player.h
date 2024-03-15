#pragma once
#include "Game.h"
#include "Item.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <string>

struct PlayerSurvivalStats
{
    float health;
    float water;
    float food;
};

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
    Player(sf::Sprite *sprite, const std::string name, const PlayerSurvivalStats survivalStats, const float baseSpeed);
    ~Player() = default;

    std::string GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;
    float GetBaseSpeed() const;
    float GetSpeed() const;
    sf::Sprite *GetSprite() const;

    PlayerMove GetMovement() const;

    void SetMovement(const PlayerMove movement);
    void SetSpeed(const float speed);

    void HandleMovement(sf::Clock &clock, const Game &game, const std::vector<std::unique_ptr<World>> &world);


    void UseItem(const std::vector<std::unique_ptr<World>> &world,
                 const std::vector<std::unique_ptr<ItemCfg>> &itemCfg,
                 std::vector<std::unique_ptr<Item>> &items);

private:
    sf::Sprite *m_sprite;
    std::string m_name;
    PlayerSurvivalStats m_survivalStats;
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_movement;
};

Player InitPlayer();
