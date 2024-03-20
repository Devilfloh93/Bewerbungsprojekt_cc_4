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
    Player(sf::Sprite *sprite,
           const std::string_view name,
           const PlayerSurvivalStats survivalStats,
           const float baseSpeed);
    ~Player() = default;

    std::string_view GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;
    float GetBaseSpeed() const;
    float GetSpeed() const;
    sf::Sprite *GetSprite() const;

    PlayerMove GetMove() const;
    PlayerMove GetLastMove() const;

    void SetMove(const PlayerMove move);
    void SetSpeed(const float speed);

    void HandleMove(sf::Clock &clock,
                    const Game &game,
                    const std::vector<std::unique_ptr<World>> &world,
                    std::vector<std::unique_ptr<Item>> &items);


    void UseItem(const std::vector<std::unique_ptr<World>> &world,
                 const std::vector<std::unique_ptr<ItemCfg>> &itemCfg,
                 std::vector<std::unique_ptr<Item>> &items);

    void DrawInventoryItems(sf::RenderWindow &window, const std::vector<std::unique_ptr<ItemCfg>> &itemCfg);

private:
    sf::Sprite *m_sprite;
    std::string m_name;
    PlayerSurvivalStats m_survivalStats;
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_move;
    PlayerMove m_lastMove;
    std::map<std::uint32_t, std::uint16_t> m_items;
};

Player InitPlayer(const std::vector<std::unique_ptr<Texture>> &textures);
