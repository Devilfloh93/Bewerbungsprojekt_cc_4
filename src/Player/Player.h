#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <World.h>
#include <array>
#include <cstdint>
#include <memory>
#include <string>

class Game;
struct PlayerSurvivalStats
{
    float health;
    float water;
    float food;
};

struct MoveAllowed
{
    bool up;
    bool down;
    bool left;
    bool right;
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
           const string_view name,
           const PlayerSurvivalStats survivalStats,
           const float baseSpeed,
           const uint8_t animID);
    ~Player() = default;

    string_view GetName() const;
    float GetHealth() const;
    float GetWater() const;
    float GetFood() const;
    float GetBaseSpeed() const;
    float GetSpeed() const;
    sf::Sprite *GetSprite() const;

    PlayerMove GetMove() const;
    PlayerMove GetLastMove() const;
    void CheckCollision(Game &game);

    void SetMove(const PlayerMove move);
    void SetSpeed(const float speed);

    void HandleMove(sf::Clock &clock, Game &game);

    void UseItem(Game &game);

    void DrawInventoryItems(sf::RenderWindow &window, const vector<ItemCfg *> &itemCfg);

private:
    sf::Sprite *m_sprite;
    string m_name;
    PlayerSurvivalStats m_survivalStats;
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_move;
    PlayerMove m_lastMove;
    map<uint32_t, uint16_t> m_items;
    uint8_t m_animID;
    MoveAllowed m_moveAllowed;
    World *m_objectInFront;
};
